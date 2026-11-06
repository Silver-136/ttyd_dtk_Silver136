// Keelhaul Key (muj): battle database: the enemy groups (BattleSetup) and stages of the island,
// the Cortez fight and Lord Crump's fight.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_sub.h"
#include "evt/evt_eff.h"

#include "battle/battle_stage_object.h"
#include "system.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "muj.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"


static USER_FUNC(_wave_offset);
extern BattleUnitKind unit_boss_cortez;
extern BattleUnitKind unit_boss_kanbu3;

BattleStageObjectData muj_btlobject_data[31];
BattleStageData muj_btlstage_data[4];
BattleUnitSetup btlparty_muj_muj_cortez[1];
BattleGroupSetup btlgrp_muj_muj_cortez;
BattleUnitSetup btlparty_muj_muj_kanbu[2];
BattleGroupSetup btlgrp_muj_muj_kanbu;
BattleUnitSetup btlparty_muj_muj_01_01[3];
BattleGroupSetup btlgrp_muj_muj_01_01;
BattleUnitSetup btlparty_muj_muj_05_01[3];
BattleGroupSetup btlgrp_muj_muj_05_01;
BattleUnitSetup btlparty_muj_muj_05_02[3];
BattleGroupSetup btlgrp_muj_muj_05_02;
BattleUnitSetup btlparty_muj_muj_00_01[3];
BattleGroupSetup btlgrp_muj_muj_00_01;
BattleUnitSetup btlparty_muj_muj_02_01_off_01[1];
BattleGroupSetup btlgrp_muj_muj_02_01_off_01;
BattleUnitSetup btlparty_muj_muj_02_01_off_02[2];
BattleGroupSetup btlgrp_muj_muj_02_01_off_02;
BattleUnitSetup btlparty_muj_muj_02_01_off_03[3];
BattleGroupSetup btlgrp_muj_muj_02_01_off_03;
BattleUnitSetup btlparty_muj_muj_02_01_off_04[4];
BattleGroupSetup btlgrp_muj_muj_02_01_off_04;
BattleUnitSetup btlparty_muj_muj_02_04_off_01[1];
BattleGroupSetup btlgrp_muj_muj_02_04_off_01;
BattleUnitSetup btlparty_muj_muj_02_04_off_02[2];
BattleGroupSetup btlgrp_muj_muj_02_04_off_02;
BattleUnitSetup btlparty_muj_muj_02_04_off_03[3];
BattleGroupSetup btlgrp_muj_muj_02_04_off_03;
BattleUnitSetup btlparty_muj_muj_02_07_off_01[2];
BattleGroupSetup btlgrp_muj_muj_02_07_off_01;
BattleUnitSetup btlparty_muj_muj_02_07_off_02[3];
BattleGroupSetup btlgrp_muj_muj_02_07_off_02;
BattleUnitSetup btlparty_muj_muj_02_07_off_03[4];
BattleGroupSetup btlgrp_muj_muj_02_07_off_03;
BattleUnitSetup btlparty_muj_muj_03_01_off_01[2];
BattleGroupSetup btlgrp_muj_muj_03_01_off_01;
BattleUnitSetup btlparty_muj_muj_03_01_off_02[3];
BattleGroupSetup btlgrp_muj_muj_03_01_off_02;
BattleUnitSetup btlparty_muj_muj_03_01_off_03[3];
BattleGroupSetup btlgrp_muj_muj_03_01_off_03;
BattleUnitSetup btlparty_muj_muj_03_02_off_01[2];
BattleGroupSetup btlgrp_muj_muj_03_02_off_01;
BattleUnitSetup btlparty_muj_muj_03_02_off_02[3];
BattleGroupSetup btlgrp_muj_muj_03_02_off_02;
BattleUnitSetup btlparty_muj_muj_03_02_off_03[3];
BattleGroupSetup btlgrp_muj_muj_03_02_off_03;
BattleUnitSetup btlparty_muj_muj_03_04_off_01[2];
BattleGroupSetup btlgrp_muj_muj_03_04_off_01;
BattleUnitSetup btlparty_muj_muj_03_04_off_02[3];
BattleGroupSetup btlgrp_muj_muj_03_04_off_02;
BattleUnitSetup btlparty_muj_muj_03_04_off_03[3];
BattleGroupSetup btlgrp_muj_muj_03_04_off_03;
NpcBattleSelect btlgrpsel_muj_btlno_muj_test1_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_test2_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_test3_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_test4_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_cortez_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_kanbu_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_01_01_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_05_01_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_05_02_off[2];
NpcBattleSelect btlgrpsel_muj_btlno_muj_02_01_off[4];
NpcBattleSelect btlgrpsel_muj_btlno_muj_02_02_off[4];
NpcBattleSelect btlgrpsel_muj_btlno_muj_02_07_off[4];
NpcBattleSelect btlgrpsel_muj_btlno_muj_03_01_off[4];
NpcBattleSelect btlgrpsel_muj_btlno_muj_03_02_off[4];
NpcBattleSelect btlgrpsel_muj_btlno_muj_03_04_off[4];
NpcSetupBattleInfo btlsetup_muj_tbl[16];
NpcTerritoryInfo btl_muj_setup_no_tbl[16];
EvtWord bgset_muj_wave_event[59];
EvtWord bgset_muj_sun_event[18];

/**
 * Scenery props of the four Keelhaul Key battle stages, in order: each entry names a model of the stage file
 * and where it stands.
 */
BattleStageObjectData muj_btlobject_data[] = {
    {"A1", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"hata", 2, STAGE_LAYER_A2, {178.0f, 0.0f, -49.0f}, 0, 60, {0}},
    {"iwa", 2, STAGE_LAYER_A2, {-183.0f, 0.0f, -28.0f}, 10, 50, {0}},
    {"zai", 2, STAGE_LAYER_A2, {126.0f, 0.0f, -26.0f}, 20, 40, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"A1", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -56.0f}, 0, 60, {0}},
    {"kusa_mae", 2, STAGE_LAYER_A2, {0.0f, 0.0f, 70.0f}, 0, 60, {0}},
    {"kusa_usiro", 2, STAGE_LAYER_A2, {0.0f, 0.0f, -44.0f}, 10, 50, {0}},
    {"kusa_01", 2, STAGE_LAYER_A2, {-79.0f, 0.0f, -28.0f}, 20, 40, {0}},
    {"kusa_02", 2, STAGE_LAYER_A2, {128.0f, 0.0f, -33.0f}, 30, 30, {0}},
    {"necco", 2, STAGE_LAYER_A2, {0.0f, 0.0f, -34.0f}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"takara_01", 2, STAGE_LAYER_A2, {111.0f, 0.0f, -69.0f}, 0, 60, {0}},
    {"takara_02", 2, STAGE_LAYER_A2, {-87.0f, 0.0f, -72.0f}, 10, 50, {0}},
    {"takara_03", 2, STAGE_LAYER_A2, {136.0f, 0.0f, 57.0f}, 20, 40, {0}},
    {"takara_04", 2, STAGE_LAYER_A2, {-182.0f, 0.0f, 57.0f}, 30, 30, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C_kabe", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"nami_00", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"nami_01", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"A1", 2, STAGE_LAYER_A1, {107.0f, 0.0f, -53.0f}, 0, 60, {0}},
    {"S_mae_l", 2, STAGE_LAYER_A2, {-90.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_mae_r", 2, STAGE_LAYER_A2, {84.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_oku_l", 2, STAGE_LAYER_A2, {-35.0f, 0.0f, -42.0f}, 0, 60, {0}},
    {"S_oku_r", 2, STAGE_LAYER_A2, {35.0f, 0.0f, -42.0f}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"if_wire", 5, STAGE_LAYER_IRON_FRAME, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, STAGE_LAYER_IRON_FRAME, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
};

/**
 * The four Keelhaul Key battle stages (stg_05_1, stg_05_2, stg_05_5 and stg_05_0), each with its slice of
 * muj_btlobject_data and the sun background script.
 */
BattleStageData muj_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_05_1", // specificStageDataDir
        6, // numObjects
        muj_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_muj_sun_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_05_2", // specificStageDataDir
        8, // numObjects
        &muj_btlobject_data[6], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_muj_sun_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_05_5", // specificStageDataDir
        8, // numObjects
        &muj_btlobject_data[14], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_05_0", // specificStageDataDir
        9, // numObjects
        &muj_btlobject_data[22], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_muj_sun_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_muj_muj_cortez[] = {
    {
        &unit_boss_cortez, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {146.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_cortez, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_cortez = {
    1, // numEnemies
    btlparty_muj_muj_cortez, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_kanbu[] = {
    {
        &unit_boss_gundan_zako_group1, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {80.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_boss_kanbu3, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {160.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kanbu1, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_kanbu = {
    2, // numEnemies
    btlparty_muj_muj_kanbu, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_01_01[] = {
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_01_01 = {
    3, // numEnemies
    btlparty_muj_muj_01_01, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_05_01[] = {
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_05_01 = {
    3, // numEnemies
    btlparty_muj_muj_05_01, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_05_02[] = {
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_05_02 = {
    3, // numEnemies
    btlparty_muj_muj_05_02, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_00_01[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_00_01 = {
    3, // numEnemies
    btlparty_muj_muj_00_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_01[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_01_off_01 = {
    1, // numEnemies
    btlparty_muj_muj_02_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_02[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_01_off_02 = {
    2, // numEnemies
    btlparty_muj_muj_02_01_off_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_03[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_01_off_03 = {
    3, // numEnemies
    btlparty_muj_muj_02_01_off_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_04[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_01_off_04 = {
    4, // numEnemies
    btlparty_muj_muj_02_01_off_04, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_04_off_01[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_04_off_01 = {
    1, // numEnemies
    btlparty_muj_muj_02_04_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_04_off_02[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_04_off_02 = {
    2, // numEnemies
    btlparty_muj_muj_02_04_off_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_04_off_03[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_04_off_03 = {
    3, // numEnemies
    btlparty_muj_muj_02_04_off_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_07_off_01[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_07_off_01 = {
    2, // numEnemies
    btlparty_muj_muj_02_07_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_07_off_02[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_07_off_02 = {
    3, // numEnemies
    btlparty_muj_muj_02_07_off_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_02_07_off_03[] = {
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_02_07_off_03 = {
    4, // numEnemies
    btlparty_muj_muj_02_07_off_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_01_off_01[] = {
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_01_off_01 = {
    2, // numEnemies
    btlparty_muj_muj_03_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_01_off_02[] = {
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_01_off_02 = {
    3, // numEnemies
    btlparty_muj_muj_03_01_off_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_01_off_03[] = {
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_01_off_03 = {
    3, // numEnemies
    btlparty_muj_muj_03_01_off_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_02_off_01[] = {
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_02_off_01 = {
    2, // numEnemies
    btlparty_muj_muj_03_02_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_02_off_02[] = {
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_02_off_02 = {
    3, // numEnemies
    btlparty_muj_muj_03_02_off_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_02_off_03[] = {
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_02_off_03 = {
    3, // numEnemies
    btlparty_muj_muj_03_02_off_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_04_off_01[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_04_off_01 = {
    2, // numEnemies
    btlparty_muj_muj_03_04_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_04_off_02[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_04_off_02 = {
    3, // numEnemies
    btlparty_muj_muj_03_04_off_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_muj_muj_03_04_off_03[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_muj_muj_03_04_off_03 = {
    3, // numEnemies
    btlparty_muj_muj_03_04_off_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_test1_off[] = {
    {50, &btlgrp_muj_muj_01_01, &muj_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_test2_off[] = {
    {50, &btlgrp_muj_muj_01_01, muj_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_test3_off[] = {
    {50, &btlgrp_muj_muj_01_01, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_test4_off[] = {
    {50, &btlgrp_muj_muj_01_01, &muj_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_cortez_off[] = {
    {50, &btlgrp_muj_muj_cortez, &muj_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_kanbu_off[] = {
    {50, &btlgrp_muj_muj_kanbu, &muj_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_01_01_off[] = {
    {50, &btlgrp_muj_muj_01_01, muj_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_05_01_off[] = {
    {50, &btlgrp_muj_muj_05_01, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_05_02_off[] = {
    {50, &btlgrp_muj_muj_05_02, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_02_01_off[] = {
    {10, &btlgrp_muj_muj_02_01_off_01, &muj_btlstage_data[1]},
    {20, &btlgrp_muj_muj_02_01_off_02, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_02_01_off_03, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_02_02_off[] = {
    {10, &btlgrp_muj_muj_02_04_off_01, &muj_btlstage_data[1]},
    {20, &btlgrp_muj_muj_02_04_off_02, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_02_04_off_03, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_02_07_off[] = {
    {10, &btlgrp_muj_muj_02_07_off_01, &muj_btlstage_data[1]},
    {20, &btlgrp_muj_muj_02_07_off_02, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_02_07_off_03, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_03_01_off[] = {
    {10, &btlgrp_muj_muj_03_01_off_01, &muj_btlstage_data[1]},
    {20, &btlgrp_muj_muj_03_01_off_02, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_03_01_off_03, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_03_02_off[] = {
    {10, &btlgrp_muj_muj_03_02_off_01, &muj_btlstage_data[1]},
    {20, &btlgrp_muj_muj_03_02_off_02, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_03_02_off_03, &muj_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_muj_btlno_muj_03_04_off[] = {
    {10, &btlgrp_muj_muj_03_04_off_01, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_03_04_off_02, &muj_btlstage_data[1]},
    {10, &btlgrp_muj_muj_03_04_off_03, &muj_btlstage_data[1]},
    {0},
};

/**
 * Encounter table of Keelhaul Key: one entry per battle number with its stage, enemy group pool, battle music
 * and random-roll ranges.
 */
NpcSetupBattleInfo btlsetup_muj_tbl[] = {
    {
        "マップstg_05_0", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_05_1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_05_2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test3_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_05_5", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test4_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "コルテス", // name
        "muj_12", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_cortez_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        6, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG5_KORTESS1", // bgmName
    },
    {
        "幹部その３", // name
        "muj_00", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_kanbu_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG3_3RDLEADER1", // bgmName
    },
    {
        "MUJ01おんりょう１", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_01_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        "MUJ01おんりょう２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_05_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        "MUJ01おんりょう３", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_05_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        "MUJ02グリーンチョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_02_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "MUJ02フラワーチョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_02_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "MUJ02グリーンチョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_02_07_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "MUJ03ポイズンパックン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_03_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "MUJ03ポイズンパックン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_03_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "MUJ03フラワーチョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_03_04_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_muj_setup_no_tbl[] = {
    {"BTLNO_MUJ_TEST1", 0},
    {"BTLNO_MUJ_TEST2", 1},
    {"BTLNO_MUJ_TEST3", 2},
    {"BTLNO_MUJ_TEST4", 3},
    {"BTLNO_MUJ_CORTEZ", 4},
    {"BTLNO_MUJ_KANBU", 5},
    {"BTLNO_MUJ_01_01", 6},
    {"BTLNO_MUJ_05_01", 7},
    {"BTLNO_MUJ_05_02", 8},
    {"BTLNO_MUJ_02_01", 9},
    {"BTLNO_MUJ_02_02", 10},
    {"BTLNO_MUJ_02_07", 11},
    {"BTLNO_MUJ_03_01", 12},
    {"BTLNO_MUJ_03_02", 13},
    {"BTLNO_MUJ_03_04", 14},
    {0},
};

/**
 * Battle background event for the beach stage: endlessly bobs the three wave layers (nami_1, nami_2, nami_3)
 * with different periods and amplitudes through _wave_offset, so the sea moves behind the fight.
 */
EVT_BEGIN(bgset_muj_wave_event)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    LABEL(0)
    CALL(_wave_offset, STRING("nami_1"), LW(2), 600, 20, LW(3), 250, 6)
    CALL(_wave_offset, STRING("nami_2"), LW(4), 1100, 40, LW(5), 360, 7)
    CALL(_wave_offset, STRING("nami_3"), LW(6), 1250, 50, LW(7), 450, 8)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/** Battle background event that adds the sunlight_n64 effect (sunlight_bt) over the beach stage. */
EVT_BEGIN(bgset_muj_sun_event)
    CALL(evt_eff64, STRING("sunlight_bt"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Script helper for bgset_muj_wave_event: moves the battle stage object named args[0] by a sine wave in x
 * (period args[2], amplitude args[3]) and y (period args[5], amplitude args[6]);
 * args[1] and args[4] are the script variables holding the two current steps,
 * which it advances and wraps each call.
 */
static USER_FUNC(_wave_offset) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);

    // Keep the variable references so the updated phases can be written back each frame.
    s32 varX = args[1];
    s32 stepX = evtGetValue(event, varX);
    s32 periodX = evtGetValue(event, args[2]);
    s32 ampX = evtGetValue(event, args[3]);
    s32 varY = args[4];
    s32 stepY = evtGetValue(event, varY);
    s32 periodY = evtGetValue(event, args[5]);
    s32 ampY = evtGetValue(event, args[6]);
    BattleStageObject* obj = BattleSearchObjectPtr(name);

    // Periods must be nonzero and the stage object present; phase division is intentionally integer.
    obj->positionOffset.x = ampX * sinfd(stepX * 360 / periodX);
    obj->positionOffset.y = ampY * sinfd(stepY * 360 / periodY + 270) + ampY;
    stepX++;
    if (stepX >= periodX) {
        stepX = 0;
    }
    evtSetValue(event, varX, stepX);
    stepY++;
    if (stepY >= periodY) {
        stepY = 0;
    }
    evtSetValue(event, varY, stepY);
    return EVT_RETURN_DONE;
}

