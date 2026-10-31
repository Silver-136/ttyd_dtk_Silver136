// tst.rel battle setups: the BTLNO_TEST_* enemy groups and the test stages (bti_01..bti_04), plus the
// background-set wave offset user function that moves stage object "B".
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "battle/battle_stage_object.h"
#include "system.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_sub.h"
#include "driver/npcdrv.h"
#include "tst.h"

static USER_FUNC(_wave_offset);
extern EvtWord bgset_tst_wave_event[];

/**
 * Stage objects of the test battle stages, in four runs: entries 0-7 (trees ki_1 to ki_3, fence saku_1,
 * background sets B and C, and the if_wire and if_body objects), 8-12 (a smaller scene of two trees, the
 * fence, B and C), and 13-18 (the tree scene again without the wire objects).
 */
BattleStageObjectData tst_btlobject_data[] = {
    {"ki_1", 0, 0, {136.0f, 0.0f, -63.0f}, 50, 40, {0}},
    {"ki_2", 0, 0, {86.0f, 0.0f, -30.0f}, 0, 90, {0}},
    {"ki_3", 0, 0, {40.0f, 0.0f, -50.0f}, 30, 60, {0}},
    {"saku_1", 0, 1, {-19.0f, 0.0f, -49.0f}, 0, 90, {0}},
    {"B", 2, 2, {0}, 0, 90, {0}},
    {"C", 2, 3, {0}, 0, 90, {0}},
    {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"ki_1", 0, 0, {136.0f, 0.0f, -63.0f}, 50, 30, {0}},
    {"ki_2", 0, 0, {40.0f, 0.0f, -50.0f}, 0, 90, {0}},
    {"saku_1", 0, 1, {-50.0f, 0.0f, -50.0f}, 0, 90, {0}},
    {"B", 2, 2, {0}, 0, 90, {0}},
    {"C", 2, 3, {0}, 0, 90, {0}},
    {"ki_1", 0, 0, {136.0f, 0.0f, -63.0f}, 50, 40, {0}},
    {"ki_2", 0, 0, {86.0f, 0.0f, -30.0f}, 0, 90, {0}},
    {"ki_3", 0, 0, {40.0f, 0.0f, -50.0f}, 30, 60, {0}},
    {"saku_1", 0, 1, {-19.0f, 0.0f, -49.0f}, 0, 90, {0}},
    {"B", 2, 2, {0}, 0, 90, {0}},
    {"C", 2, 3, {0}, 0, 90, {0}},
};

/**
 * Test battle stages used only by the BTLNO_TEST_* fights: [0] is empty (no objects), [1] to [4] are the
 * bti_01 to bti_04 variants (stg_01_0 to stg_01_3) with the first six objects, [5] swaps in objects 13-18
 * and runs bgset_tst_wave_event to sway background set B, and [6] uses all eight objects of the first run.
 */
BattleStageData tst_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_01_0", // specificStageDataDir
        0, // numObjects
        NULL, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_0", // specificStageDataDir
        6, // numObjects
        tst_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_02", // globalStageDataDir
        "stg_01_1", // specificStageDataDir
        6, // numObjects
        tst_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_03", // globalStageDataDir
        "stg_01_2", // specificStageDataDir
        6, // numObjects
        tst_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_04", // globalStageDataDir
        "stg_01_3", // specificStageDataDir
        6, // numObjects
        tst_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_0", // specificStageDataDir
        6, // numObjects
        &tst_btlobject_data[13], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, bgset_tst_wave_event, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_0", // specificStageDataDir
        8, // numObjects
        tst_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

/** Enemy lineup of test battle BTLNO_TEST_KURIBOO_1: Goomba. */
BattleUnitSetup btlparty_test_kuriboo_1[] = {
    {
        &unit_kuriboo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinabita_kinoko, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_KURIBOO_1: Goomba, with its item drop odds and heart and flower
 * drop tables.
 */
BattleGroupSetup btlgrp_test_kuriboo_1 = {
    1, // numEnemies
    btlparty_test_kuriboo_1, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_KURIBOO: Goomba, Spiky Goomba, Paragoomba. */
BattleUnitSetup btlparty_test_kuriboo[] = {
    {
        &unit_kuriboo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinabita_kinoko, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kirakira_otoshi, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_koorino_ibuki, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_KURIBOO: Goomba, Spiky Goomba, Paragoomba, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_kuriboo = {
    3, // numEnemies
    btlparty_test_kuriboo, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_HYPER_KURIBOO: Hyper Goomba, H. S. Goomba, H. Paragoomba. */
BattleUnitSetup btlparty_test_hyper_kuriboo[] = {
    {
        &unit_hyper_kuriboo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_fire_flower, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yurayura_jishin, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_suitooru, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_HYPER_KURIBOO: Hyper Goomba, H. S. Goomba, H. Paragoomba, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_hyper_kuriboo = {
    3, // numEnemies
    btlparty_test_hyper_kuriboo, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_YAMIKURIBOO: Gloomba, Spiky Gloomba, Paragloomba. */
BattleUnitSetup btlparty_test_yamikuriboo[] = {
    {
        &unit_yami_kuriboo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dekadeka_drink, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_yami_patakuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_YAMIKURIBOO: Gloomba, Spiky Gloomba, Paragloomba, with its item
 * drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_yamikuriboo = {
    3, // numEnemies
    btlparty_test_yamikuriboo, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_NOKONOKO_YAMINOKO: Koopa Troopa, Paratroopa, Dark Koopa, D.
 * Paratroopa.
 */
BattleUnitSetup btlparty_test_nokonoko_yaminoko[] = {
    {
        &unit_nokonoko, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_patapata, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_yami_noko, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pow_block, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_funyafunya_kun, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_NOKONOKO_YAMINOKO: Koopa Troopa, Paratroopa, Dark Koopa, D.
 * Paratroopa, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_nokonoko_yaminoko = {
    4, // numEnemies
    btlparty_test_nokonoko_yaminoko, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_URA_NOKO_TOGE_NOKO: Shady Koopa, S. Paratroopa, Koopatrol, D.
 * Koopatrol.
 */
BattleUnitSetup btlparty_test_ura_noko_toge_noko[] = {
    {
        &unit_ura_noko, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_ura_pata, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_togenoko, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_URA_NOKO_TOGE_NOKO: Shady Koopa, S. Paratroopa, Koopatrol, D.
 * Koopatrol, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_ura_noko_toge_noko = {
    4, // numEnemies
    btlparty_test_ura_noko_toge_noko, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_HONENOKO_FIGHTER: Dull Bones, Dry Bones, KP Koopa, KP Paratroopa.
 */
BattleUnitSetup btlparty_test_honenoko_fighter[] = {
    {
        &unit_honenoko, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_karon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_nokonoko_fighter, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_patapata_fighter, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_HONENOKO_FIGHTER: Dull Bones, Dry Bones, KP Koopa, KP Paratroopa,
 * with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_honenoko_fighter = {
    4, // numEnemies
    btlparty_test_honenoko_fighter, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_BASABASA: Swooper x2, Swoopula x2, Swampire. */
BattleUnitSetup btlparty_test_basabasa[] = {
    {
        &unit_basabasa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 130.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_BASABASA: Swooper x2, Swoopula x2, Swampire, with its item drop
 * odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_basabasa = {
    5, // numEnemies
    btlparty_test_basabasa, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_CHOROBON: Fuzzy, Green Fuzzy, Flower Fuzzy x2. */
BattleUnitSetup btlparty_test_chorobon[] = {
    {
        &unit_chorobon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_CHOROBON: Fuzzy, Green Fuzzy, Flower Fuzzy x2, with its item drop
 * odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_chorobon = {
    4, // numEnemies
    btlparty_test_chorobon, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_SINEMON: Bald Cleft, Moon Cleft, Cleft, H. Bald Cleft, Hyper
 * Cleft.
 */
BattleUnitSetup btlparty_test_sinemon[] = {
    {
        &unit_sinnosuke, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hyper_sinnosuke, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_SINEMON: Bald Cleft, Moon Cleft, Cleft, H. Bald Cleft, Hyper
 * Cleft, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_sinemon = {
    5, // numEnemies
    btlparty_test_sinemon, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_MET: Buzzy Beetle, Spike Top, Parabuzzy, S. Parabuzzy. */
BattleUnitSetup btlparty_test_met[] = {
    {
        &unit_met, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_togemet, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_MET: Buzzy Beetle, Spike Top, Parabuzzy, S. Parabuzzy, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_met = {
    4, // numEnemies
    btlparty_test_met, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_CRIMSON_MET: Red Buzzy, R. S. Buzzy, R. Parabuzzy x2. */
BattleUnitSetup btlparty_test_crimson_met[] = {
    {
        &unit_crimson_met, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_crimson_togemet, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_crimson_patamet, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_crimson_patatogemet, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_CRIMSON_MET: Red Buzzy, R. S. Buzzy, R. Parabuzzy x2, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_crimson_met = {
    4, // numEnemies
    btlparty_test_crimson_met, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_PANSY: Crazee Dayzee x2, Amazy Dayzee x2. */
BattleUnitSetup btlparty_test_pansy[] = {
    {
        &unit_pansy, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_PANSY: Crazee Dayzee x2, Amazy Dayzee x2, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_pansy = {
    4, // numEnemies
    btlparty_test_pansy, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_BOMHEI: Bob-omb x4. */
BattleUnitSetup btlparty_test_bomhei[] = {
    {
        &unit_bomhei, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_BOMHEI: Bob-omb x4, with its item drop odds and heart and flower
 * drop tables.
 */
BattleGroupSetup btlgrp_test_bomhei = {
    4, // numEnemies
    btlparty_test_bomhei, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_HEABY_BOMB: Bulky Bob-omb x2, Bob-ulk x2. */
BattleUnitSetup btlparty_test_heaby_bomb[] = {
    {
        &unit_heavy_bom, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_HEABY_BOMB: Bulky Bob-omb x2, Bob-ulk x2, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_heaby_bomb = {
    4, // numEnemies
    btlparty_test_heaby_bomb, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_KILLER: Bullet Bill, Bombshell Bill, Bill Blaster, B. Bill
 * Blaster.
 */
BattleUnitSetup btlparty_test_killer[] = {
    {
        &unit_killer, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_super_killer, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_killer_cannon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kinoko, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kinoko, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_KILLER: Bullet Bill, Bombshell Bill, Bill Blaster, B. Bill
 * Blaster, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_killer = {
    4, // numEnemies
    btlparty_test_killer, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_PAKKUN_FLOWER: Piranha Plant x2, Putrid Piranha x2, Frost Piranha.
 */
BattleUnitSetup btlparty_test_pakkun_flower[] = {
    {
        &unit_pakkun_flower, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_poison_pakkun, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_ice_pakkun, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_PAKKUN_FLOWER: Piranha Plant x2, Putrid Piranha x2, Frost Piranha,
 * with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_pakkun_flower = {
    5, // numEnemies
    btlparty_test_pakkun_flower, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_TOGEDARUMA: Bristle x2, Dark Bristle x2. */
BattleUnitSetup btlparty_test_togedaruma[] = {
    {
        &unit_togedaruma, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_togedaruma, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_TOGEDARUMA: Bristle x2, Dark Bristle x2, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_togedaruma = {
    4, // numEnemies
    btlparty_test_togedaruma, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_HANNYA: Spinia, Spania x2, Spunia. */
BattleUnitSetup btlparty_test_hannya[] = {
    {
        &unit_hinnya, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_HANNYA: Spinia, Spania x2, Spunia, with its item drop odds and
 * heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_hannya = {
    4, // numEnemies
    btlparty_test_hannya, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_SAMBO: Pokey x2, Poison Pokey x2. */
BattleUnitSetup btlparty_test_sambo[] = {
    {
        &unit_sambo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_SAMBO: Pokey x2, Poison Pokey x2, with its item drop odds and
 * heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_sambo = {
    4, // numEnemies
    btlparty_test_sambo, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_BROS: Hammer Bro, Boomerang Bro, Fire Bro, Bob-omb. */
BattleUnitSetup btlparty_test_bros[] = {
    {
        &unit_hammer_bros, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_boomerang_bros, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_fire_bros, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_BROS: Hammer Bro, Boomerang Bro, Fire Bro, Bob-omb, with its item
 * drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_bros = {
    4, // numEnemies
    btlparty_test_bros, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_TOGEZO_JYUGEM: Spiny, Lakitu, S. Blue Spiny, Dark Lakitu. */
BattleUnitSetup btlparty_test_togezo_jyugem[] = {
    {
        &unit_togezo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_jyugem, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hyper_togezo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_TOGEZO_JYUGEM: Spiny, Lakitu, S. Blue Spiny, Dark Lakitu, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_togezo_jyugem = {
    4, // numEnemies
    btlparty_test_togezo_jyugem, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_WANWAN: Chain-Chomp x4. */
BattleUnitSetup btlparty_test_wanwan[] = {
    {
        &unit_wanwan, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_WANWAN: Chain-Chomp x4, with its item drop odds and heart and
 * flower drop tables.
 */
BattleGroupSetup btlgrp_test_wanwan = {
    4, // numEnemies
    btlparty_test_wanwan, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_KUROKUMORN: Ruff Puff, Poison Puff, Ice Puff, Dark Puff. */
BattleUnitSetup btlparty_test_kurokumorn[] = {
    {
        &unit_kurokumorn, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_KUROKUMORN: Ruff Puff, Poison Puff, Ice Puff, Dark Puff, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_kurokumorn = {
    4, // numEnemies
    btlparty_test_kurokumorn, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_PIDERS: Pider x2, Arantula x2. */
BattleUnitSetup btlparty_test_piders[] = {
    {
        &unit_piders, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_PIDERS: Pider x2, Arantula x2, with its item drop odds and heart
 * and flower drop tables.
 */
BattleGroupSetup btlgrp_test_piders = {
    4, // numEnemies
    btlparty_test_piders, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_BUBBLE: Lava Bubble, Ember, Phantom Ember. */
BattleUnitSetup btlparty_test_bubble[] = {
    {
        &unit_bubble, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hermos, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_BUBBLE: Lava Bubble, Ember, Phantom Ember, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_bubble = {
    3, // numEnemies
    btlparty_test_bubble, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_MONBAN: Swooper, Gus, Paratroopa, Dark Craw. */
BattleUnitSetup btlparty_test_monban[] = {
    {
        &unit_basabasa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_monban, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_patapata, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_MONBAN: Swooper, Gus, Paratroopa, Dark Craw, with its item drop
 * odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_monban = {
    4, // numEnemies
    btlparty_test_monban, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_BORODO: Bandit, Badge Bandit, Big Bandit. */
BattleUnitSetup btlparty_test_borodo[] = {
    {
        &unit_borodo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_badge_borodo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_borodo_king, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_BORODO: Bandit, Badge Bandit, Big Bandit, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_borodo = {
    3, // numEnemies
    btlparty_test_borodo, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_TAMURA: Yux, Pokey, Hyper Cleft x2, B. Bill Blaster. */
BattleUnitSetup btlparty_test_tamura[] = {
    {
        &unit_barriern, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 40.0f, -20.0f}, // position
        0, // hitOffsetX
        {2, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_TAMURA: Yux, Pokey, Hyper Cleft x2, B. Bill Blaster, with its item
 * drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_tamura = {
    5, // numEnemies
    btlparty_test_tamura, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_KISHI: Pale Piranha x2. */
BattleUnitSetup btlparty_test_kishi[] = {
    {
        &unit_monochrome_pakkun, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {2, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_KISHI: Pale Piranha x2, with its item drop odds and heart and
 * flower drop tables.
 */
BattleGroupSetup btlgrp_test_kishi = {
    2, // numEnemies
    btlparty_test_kishi, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_TAMURA2: Pennington. */
BattleUnitSetup btlparty_test_tamura2[] = {
    {
        &unit_powan, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_TAMURA2: Pennington, with its item drop odds and heart and flower
 * drop tables.
 */
BattleGroupSetup btlgrp_test_tamura2 = {
    1, // numEnemies
    btlparty_test_tamura2, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_NAKAYAMA: Wanawana x3. */
BattleUnitSetup btlparty_test_nakayama[] = {
    {
        &unit_wanawana, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_wanawana, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_wanawana, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_NAKAYAMA: Wanawana x3, with its item drop odds and heart and
 * flower drop tables.
 */
BattleGroupSetup btlgrp_test_nakayama = {
    3, // numEnemies
    btlparty_test_nakayama, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_BARRIERN: Z-Yux, X-Yux. */
BattleUnitSetup btlparty_test_barriern[] = {
    {
        &unit_barriern_z, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_US
        {90.0f, 60.0f, 0.0f}, // position
#else
        {50.0f, 40.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {4, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_barriern_custom, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_US
        {170.0f, 60.0f, 0.0f}, // position
#else
        {130.0f, 40.0f, 10.0f}, // position
#endif
        0, // hitOffsetX
        {4, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_BARRIERN: Z-Yux, X-Yux, with its item drop odds and heart and
 * flower drop tables.
 */
BattleGroupSetup btlgrp_test_barriern = {
    2, // numEnemies
    btlparty_test_barriern, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of test battle BTLNO_TEST_VRIETY_HEIGHT: Buzzy Beetle, Lava Bubble, Ruff Puff, Paragloomba,
 * Swooper.
 */
BattleUnitSetup btlparty_test_vriety_height[] = {
    {
        &unit_met, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_kurokumorn, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_yami_patakuri, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_VRIETY_HEIGHT: Buzzy Beetle, Lava Bubble, Ruff Puff, Paragloomba,
 * Swooper, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_vriety_height = {
    5, // numEnemies
    btlparty_test_vriety_height, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_TERESA: Boo x2, Dark Boo, Goomba. */
BattleUnitSetup btlparty_test_teresa[] = {
    {
        &unit_teresa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_TERESA: Boo x2, Dark Boo, Goomba, with its item drop odds and
 * heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_teresa = {
    4, // numEnemies
    btlparty_test_teresa, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_DAISAN: X-Naut, X-Naut PhD, Elite X-Naut. */
BattleUnitSetup btlparty_test_daisan[] = {
    {
        &unit_gundan_zako, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_DAISAN: X-Naut, X-Naut PhD, Elite X-Naut, with its item drop odds
 * and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_daisan = {
    3, // numEnemies
    btlparty_test_daisan, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_MAHORN: Wizzerd, Dark Wizzerd, Elite Wizzerd. */
BattleUnitSetup btlparty_test_mahorn[] = {
    {
        &unit_mahorn, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {40.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_super_mahorn, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {100.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_MAHORN: Wizzerd, Dark Wizzerd, Elite Wizzerd, with its item drop
 * odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_mahorn = {
    3, // numEnemies
    btlparty_test_mahorn, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of test battle BTLNO_TEST_KAMEC: Magikoopa, R. Magikoopa, W. Magikoopa, G. Magikoopa. */
BattleUnitSetup btlparty_test_kamec[] = {
    {
        &unit_kamec, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_kamec_red, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_kamec_white, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
    {
        &unit_kamec_green, // unitKindParams
        1, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

/**
 * Battle group of test battle BTLNO_TEST_KAMEC: Magikoopa, R. Magikoopa, W. Magikoopa, G. Magikoopa, with
 * its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_test_kamec = {
    4, // numEnemies
    btlparty_test_kamec, // enemyData
    0, // heldItemWeight
    0, // randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Group choice for BTLNO_TEST_KURIBOO_1: the one test group (weight 10) on test stage [5]. */
NpcBattleSelect btlgrpsel_test_kuriboo_1[] = {
    {10, &btlgrp_test_kuriboo_1, &tst_btlstage_data[5]},
    {0},
};

/** Group choice for BTLNO_TEST_KURIBOO: the one test group (weight 10) on test stage [5]. */
NpcBattleSelect btlgrpsel_test_kuriboo[] = {
    {10, &btlgrp_test_kuriboo, &tst_btlstage_data[5]},
    {0},
};

/** Group choice for BTLNO_TEST_HYPER_KURIBOO: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_hyper_kuriboo[] = {
    {10, &btlgrp_test_hyper_kuriboo, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_YAMIKURIBOO: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_yamikuriboo[] = {
    {10, &btlgrp_test_yamikuriboo, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_NOKONOKO_YAMINOKO: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_nokonoko_yaminoko[] = {
    {10, &btlgrp_test_nokonoko_yaminoko, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_URA_NOKO_TOGE_NOKO: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_ura_noko_toge_noko[] = {
    {10, &btlgrp_test_ura_noko_toge_noko, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_HONENOKO_FIGHTER: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_honenoko_fighter[] = {
    {10, &btlgrp_test_honenoko_fighter, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_BASABASA: the one test group (weight 10) on test stage [6]. */
NpcBattleSelect btlgrpsel_test_basabasa[] = {
    {10, &btlgrp_test_basabasa, &tst_btlstage_data[6]},
    {0},
};

/** Group choice for BTLNO_TEST_CHOROBON: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_chorobon[] = {
    {10, &btlgrp_test_chorobon, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_SINEMON: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_sinemon[] = {
    {10, &btlgrp_test_sinemon, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_MET: the one test group (weight 10) on test stage [6]. */
NpcBattleSelect btlgrpsel_test_met[] = {
    {10, &btlgrp_test_met, &tst_btlstage_data[6]},
    {0},
};

/** Group choice for BTLNO_TEST_CRIMSON_MET: the one test group (weight 10) on test stage [6]. */
NpcBattleSelect btlgrpsel_test_crimson_met[] = {
    {10, &btlgrp_test_crimson_met, &tst_btlstage_data[6]},
    {0},
};

/** Group choice for BTLNO_TEST_PANSY: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_pansy[] = {
    {10, &btlgrp_test_pansy, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_BOMHEI: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_bomhei[] = {
    {10, &btlgrp_test_bomhei, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_HEABY_BOMB: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_heaby_bomb[] = {
    {10, &btlgrp_test_heaby_bomb, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_KILLER: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_killer[] = {
    {10, &btlgrp_test_killer, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_PAKKUN_FLOWER: the one test group (weight 10) on test stage [6]. */
NpcBattleSelect btlgrpsel_test_pakkun_flower[] = {
    {10, &btlgrp_test_pakkun_flower, &tst_btlstage_data[6]},
    {0},
};

/** Group choice for BTLNO_TEST_TOGEDARUMA: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_togedaruma[] = {
    {10, &btlgrp_test_togedaruma, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_HANNYA: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_hannya[] = {
    {10, &btlgrp_test_hannya, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_SAMBO: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_sambo[] = {
    {10, &btlgrp_test_sambo, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_BROS: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_bros[] = {
    {10, &btlgrp_test_bros, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_TOGEZO_JYUGEM: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_togezo_jyugem[] = {
    {10, &btlgrp_test_togezo_jyugem, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_WANWAN: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_wanwan[] = {
    {10, &btlgrp_test_wanwan, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_KUROKUMORN: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_kurokumorn[] = {
    {10, &btlgrp_test_kurokumorn, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_PIDERS: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_piders[] = {
    {10, &btlgrp_test_piders, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_BUBBLE: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_bubble[] = {
    {10, &btlgrp_test_bubble, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_BARRIERN: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_barriern[] = {
    {10, &btlgrp_test_barriern, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_VRIETY_HEIGHT: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_vriety_height[] = {
    {10, &btlgrp_test_vriety_height, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_TERESA: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_teresa[] = {
    {10, &btlgrp_test_teresa, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_DAISAN: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_daisan[] = {
    {10, &btlgrp_test_daisan, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_MAHORN: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_mahorn[] = {
    {10, &btlgrp_test_mahorn, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_KAMEC: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_kamec[] = {
    {10, &btlgrp_test_kamec, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_MONBAN: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_monban[] = {
    {10, &btlgrp_test_monban, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_BORODO: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_borodo[] = {
    {10, &btlgrp_test_borodo, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_TAMURA: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_tamura[] = {
    {10, &btlgrp_test_tamura, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_TAMURA2: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_tamura2[] = {
    {10, &btlgrp_test_tamura2, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_NAKAYAMA: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_nakayama[] = {
    {10, &btlgrp_test_nakayama, &tst_btlstage_data[1]},
    {0},
};

/** Group choice for BTLNO_TEST_KISHI: the one test group (weight 10) on test stage [1]. */
NpcBattleSelect btlgrpsel_test_kishi[] = {
    {10, &btlgrp_test_kishi, &tst_btlstage_data[1]},
    {0},
};

/**
 * Encounter table of tst.rel, indexed by BTLNO_TEST_*: 38 debug fights, one per enemy family (Goombas,
 * Koopas, Buzzies, Bob-ombs, Magikoopas and so on) plus a few developer test rooms. Each entry has one
 * group choice, the shared battle music BGM_ZAKO_BATTLE1 and a fixed random placement range. Registered
 * with relSetBtlAddr in tst.c.
 */
NpcSetupBattleInfo btlsetup_tst_tbl[] = {
    {
        "クリボー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_kuriboo_1, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "クリボーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_kuriboo, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ハイパークリボーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_hyper_kuriboo, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミクリボーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_yamikuriboo, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ノコノコけいとヤミノコけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_nokonoko_yaminoko, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ウラノコけいとトゲノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_ura_noko_toge_noko, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ホネノコけいとファイターなノコノコけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_honenoko_fighter, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バサバサけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_basabasa, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "チョロボンけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_chorobon, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "シンエモンけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_sinemon, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "メットけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_met, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "クリムゾンメットけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_crimson_met, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パンジーさんけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_pansy, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ボムへい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_bomhei, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヘビーボム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_heaby_bomb, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "キラーとたいほう", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_killer, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パックンフラワーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_pakkun_flower, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "トゲダルマーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_togedaruma, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ハンニャーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_hannya, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "サンボけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_sambo, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ブロスけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_bros, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "トゲゾーとジュゲム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_togezo_jyugem, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ワンワン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_wanwan, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "クモクモーンけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_kurokumorn, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パイダースけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_piders, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バブルけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_bubble, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バリアーンけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_barriern, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "さまざまな高さ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_vriety_height, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "テレサけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_teresa, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "第三勢力けい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_daisan, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マホーンけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_mahorn, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "カメックけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_kamec, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "もんばんけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_monban, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ボロドーけい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_borodo, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "たむらてすと", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_tamura, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "たむらてすと連戦テスト", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_tamura2, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "なかやまテスト", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_nakayama, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "きしてすと", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_test_kishi, // select
        0, 0, // battleFlags, randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

/**
 * Maps the BTLNO_TEST_* battle ids to their index in btlsetup_tst_tbl; registered with relSetBtlAddr in
 * tst.c.
 */
NpcTerritoryInfo btl_tst_setup_no_tbl[] = {
    {"BTLNO_TEST_KURIBOO_1", 0},
    {"BTLNO_TEST_KURIBOO", 1},
    {"BTLNO_TEST_HYPER_KURIBOO", 2},
    {"BTLNO_TEST_YAMIKURIBOO", 3},
    {"BTLNO_TEST_NOKONOKO_YAMINOKO", 4},
    {"BTLNO_TEST_URA_NOKO_TOGE_NOKO", 5},
    {"BTLNO_TEST_HONENOKO_FIGHTER", 6},
    {"BTLNO_TEST_BASABASA", 7},
    {"BTLNO_TEST_CHOROBON", 8},
    {"BTLNO_TEST_SINEMON", 9},
    {"BTLNO_TEST_MET", 10},
    {"BTLNO_TEST_CRIMSON_MET", 11},
    {"BTLNO_TEST_PANSY", 12},
    {"BTLNO_TEST_BOMHEI", 13},
    {"BTLNO_TEST_HEABY_BOMB", 14},
    {"BTLNO_TEST_KILLER", 15},
    {"BTLNO_TEST_PAKKUN_FLOWER", 16},
    {"BTLNO_TEST_TOGEDARUMA", 17},
    {"BTLNO_TEST_HANNYA", 18},
    {"BTLNO_TEST_SAMBO", 19},
    {"BTLNO_TEST_BROS", 20},
    {"BTLNO_TEST_TOGEZO_JYUGEM", 21},
    {"BTLNO_TEST_WANWAN", 22},
    {"BTLNO_TEST_KUROKUMORN", 23},
    {"BTLNO_TEST_PIDERS", 24},
    {"BTLNO_TEST_BUBBLE", 25},
    {"BTLNO_TEST_BARRIERN", 26},
    {"BTLNO_TEST_VRIETY_HEIGHT", 27},
    {"BTLNO_TEST_TERESA", 28},
    {"BTLNO_TEST_DAISAN", 29},
    {"BTLNO_TEST_MAHORN", 30},
    {"BTLNO_TEST_KAMEC", 31},
    {"BTLNO_TEST_MONBAN", 32},
    {"BTLNO_TEST_BORODO", 33},
    {"BTLNO_TEST_TAMURA", 34},
    {"BTLNO_TEST_TAMURA2", 35},
    {"BTLNO_TEST_NAKAYAMA", 36},
    {"BTLNO_TEST_KISHI", 37},
    {0},
};

/**
 * Moves a stage object along two sine waves: x = ampX * sin(stepX / periodX), y = ampY * (1 + sin(...)).
 * Args: object name, x step var, x period, x amplitude, y step var, y period, y amplitude.
 */
static USER_FUNC(_wave_offset) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 varX = args[1];
    s32 stepX = evtGetValue(event, varX);
    s32 periodX = evtGetValue(event, args[2]);
    s32 ampX = evtGetValue(event, args[3]);
    s32 varY = args[4];
    s32 stepY = evtGetValue(event, varY);
    s32 periodY = evtGetValue(event, args[5]);
    s32 ampY = evtGetValue(event, args[6]);
    BattleStageObject* pObject = BattleSearchObjectPtr(name);

#if VERSION >= VERSION_JP
    if (!pObject) {
        __assert2("battle_bgset_test.c", 102, "pObject",
                  /*"制御しようとする背景セットBの%sは存在しない"*/
                  "\x90\xA7\x8C\xE4\x82\xB5\x82\xE6\x82\xA4\x82\xC6\x82\xB7\x82\xE9\x94\x77\x8C\x69\x83\x5A\x83\x62"
                  "\x83\x67\x42\x82\xCC\x25\x73\x82\xCD\x91\xB6\x8D\xDD\x82\xB5\x82\xC8\x82\xA2",
                  name);
    }
#endif
    pObject->positionOffset.x = ampX * sinfd(stepX * 360 / periodX);
    pObject->positionOffset.y = ampY * sinfd(stepY * 360 / periodY + 270) + ampY;
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

/**
 * Endless loop for the test battle stage: each frame calls _wave_offset on stage object B to make it sway on a
 * wave (800-unit x period, 300-unit y period).
 */
EVT_BEGIN(bgset_tst_wave_event)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    LABEL(0)
    CALL(_wave_offset, STRING("B"), LW(0), 800, 30, LW(1), 300, 10)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()
