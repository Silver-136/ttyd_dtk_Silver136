// battle_database_tou: Glitz Pit battle data: arena stage layouts and the ranked match groups (tou_rank_N)
// with their unit setups, plus the field battle selection tables.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "tou2.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"
#include "evt/evt_map.h"
#include "driver/mapdrv.h"

extern BattleUnitKind unit_jyugem;

static BattleStageObjectData tou_btlobject_data[14];
static BattleStageData tou_btlstage_data[2];
static BattleUnitSetup btlparty_tou_tou_rank_19[5];
static BattleGroupSetup btlgrp_tou_tou_rank_19;
static BattleUnitSetup btlparty_tou_tou_rank_18[3];
static BattleGroupSetup btlgrp_tou_tou_rank_18;
static BattleUnitSetup btlparty_tou_tou_rank_17[3];
static BattleGroupSetup btlgrp_tou_tou_rank_17;
static BattleUnitSetup btlparty_tou_tou_rank_16[3];
static BattleGroupSetup btlgrp_tou_tou_rank_16;
static BattleUnitSetup btlparty_tou_tou_rank_15[3];
static BattleGroupSetup btlgrp_tou_tou_rank_15;
static BattleUnitSetup btlparty_tou_tou_rank_14[3];
static BattleGroupSetup btlgrp_tou_tou_rank_14;
static BattleUnitSetup btlparty_tou_tou_rank_13[3];
static BattleGroupSetup btlgrp_tou_tou_rank_13;
static BattleUnitSetup btlparty_tou_tou_rank_12[4];
static BattleGroupSetup btlgrp_tou_tou_rank_12;
static BattleUnitSetup btlparty_tou_tou_rank_11[4];
static BattleGroupSetup btlgrp_tou_tou_rank_11;
static BattleUnitSetup btlparty_tou_tou_rank_10[2];
static BattleGroupSetup btlgrp_tou_tou_rank_10;
static BattleUnitSetup btlparty_tou_tou_rank_9[2];
static BattleGroupSetup btlgrp_tou_tou_rank_9;
static BattleUnitSetup btlparty_tou_tou_rank_8[2];
static BattleGroupSetup btlgrp_tou_tou_rank_8;
static BattleUnitSetup btlparty_tou_tou_rank_7[3];
static BattleGroupSetup btlgrp_tou_tou_rank_7;
static BattleUnitSetup btlparty_tou_tou_rank_6[3];
static BattleGroupSetup btlgrp_tou_tou_rank_6;
static BattleUnitSetup btlparty_tou_tou_rank_5[3];
static BattleGroupSetup btlgrp_tou_tou_rank_5;
static BattleUnitSetup btlparty_tou_tou_rank_4[1];
static BattleGroupSetup btlgrp_tou_tou_rank_4;
static BattleUnitSetup btlparty_tou_tou_rank_3[3];
static BattleGroupSetup btlgrp_tou_tou_rank_3;
static BattleUnitSetup btlparty_tou_tou_rank_2[2];
static BattleGroupSetup btlgrp_tou_tou_rank_2;
static BattleUnitSetup btlparty_tou_tou_rank_1[1];
static BattleGroupSetup btlgrp_tou_tou_rank_1;
static BattleUnitSetup btlparty_tou_tou_add_1[4];
static BattleGroupSetup btlgrp_tou_tou_add_1;
static BattleUnitSetup btlparty_tou_tou_add_2[4];
static BattleGroupSetup btlgrp_tou_tou_add_2;
static BattleUnitSetup btlparty_tou_tou_boss[1];
static BattleGroupSetup btlgrp_tou_tou_boss;
static BattleUnitSetup btlparty_tou_tou_champ[1];
static BattleGroupSetup btlgrp_tou_tou_champ;
static BattleUnitSetup btlparty_tou_tou_koopa[1];
static BattleGroupSetup btlgrp_tou_tou_koopa;
static NpcBattleSelect btlgrpsel_tou_btlno_tou_test1_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_01_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_02_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_03_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_04_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_05_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_06_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_07_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_08_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_09_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_10_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_11_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_12_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_13_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_14_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_15_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_16_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_17_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_18_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_19_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_20_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_21_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_22_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_23_off[2];
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_24_off[2];
NpcSetupBattleInfo btlsetup_tou_tbl[26];
NpcTerritoryInfo btl_tou_setup_no_tbl[26];
EvtWord bgset_tou_screen_evt[22];

/**
 * Stage objects of the Glitz Pit arena (stg_03_0), in two sets: entries 0-5 for the normal arena (the big
 * screen "gamen", the "etc" scenery, side walls side__L and side__R, and background sets B and C) and
 * entries 6-13 for the title-match arena, the same objects plus the if_wire and if_body objects.
 */
static BattleStageObjectData tou_btlobject_data[] = {
    {"gamen", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"etc", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -54.0f}, 10, 50, {0}},
    {"side__L", 2, STAGE_LAYER_A1, {-137.0f, 0.0f, -40.0f}, 20, 40, {0}},
    {"side__R", 2, STAGE_LAYER_A1, {137.0f, 0.0f, -40.0f}, 30, 30, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"gamen", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"etc", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -54.0f}, 10, 50, {0}},
    {"side__L", 2, STAGE_LAYER_A1, {-137.0f, 0.0f, -40.0f}, 20, 40, {0}},
    {"side__R", 2, STAGE_LAYER_A1, {137.0f, 0.0f, -40.0f}, 30, 30, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"if_wire", 5, STAGE_LAYER_IRON_FRAME, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, STAGE_LAYER_IRON_FRAME, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
};

/**
 * The two Glitz Pit battle stages: [0] is the normal arena (objects 0-5), [1] is the title-match arena used
 * for the Rawk Hawk fight (objects 6-13). Both run bgset_tou_screen_evt to drive the big screen and keep
 * the background sets from being destroyed.
 */
static BattleStageData tou_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_03_0", // specificStageDataDir
        6, // numObjects
        tou_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_tou_screen_evt, // bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_03_0", // specificStageDataDir
        8, // numObjects
        &tou_btlobject_data[6], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_tou_screen_evt, // bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
};

/** Enemy lineup of the Goomba Bros. match (BTLNO_TOU_01_01): Goomba Bros. x5. */
static BattleUnitSetup btlparty_tou_tou_rank_19[] = {
    {
        &unit_kurikuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
};

/**
 * Battle group of the Goomba Bros. match (BTLNO_TOU_01_01): Goomba Bros. x5, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_19 = {
    5, // numEnemies
    btlparty_tou_tou_rank_19, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the KP Koopas match (BTLNO_TOU_01_02): KP Koopa x2, KP Paratroopa. */
static BattleUnitSetup btlparty_tou_tou_rank_18[] = {
    {
        &unit_nokonoko_fighter, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko_fighter, // itemDropTable
    },
    {
        &unit_nokonoko_fighter, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko_fighter, // itemDropTable
    },
    {
        &unit_patapata_fighter, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_patapata_fighter, // itemDropTable
    },
};

/**
 * Battle group of the KP Koopas match (BTLNO_TOU_01_02): KP Koopa x2, KP Paratroopa, with its item drop
 * odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_18 = {
    3, // numEnemies
    btlparty_tou_tou_rank_18, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Pokey Triplets match (BTLNO_TOU_01_03): Pokey x3. */
static BattleUnitSetup btlparty_tou_tou_rank_17[] = {
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

/**
 * Battle group of the Pokey Triplets match (BTLNO_TOU_01_03): Pokey x3, with its item drop odds and heart
 * and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_17 = {
    3, // numEnemies
    btlparty_tou_tou_rank_17, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Dead Bones match (BTLNO_TOU_01_04): Dull Bones x3. */
static BattleUnitSetup btlparty_tou_tou_rank_16[] = {
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

/**
 * Battle group of the Dead Bones match (BTLNO_TOU_01_04): Dull Bones x3, with its item drop odds and heart
 * and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_16 = {
    3, // numEnemies
    btlparty_tou_tou_rank_16, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Spike Storm match (BTLNO_TOU_01_05): Spiny x2, Lakitu. */
static BattleUnitSetup btlparty_tou_tou_rank_15[] = {
    {
        &unit_togezo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_togezo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

/**
 * Battle group of the Spike Storm match (BTLNO_TOU_01_05): Spiny x2, Lakitu, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_15 = {
    3, // numEnemies
    btlparty_tou_tou_rank_15, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Mind-Bogglers match (BTLNO_TOU_01_06): Pale Piranha, Dark Puff, Pider. */
static BattleUnitSetup btlparty_tou_tou_rank_14[] = {
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togezo, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togezo, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_togezo, // itemDropTable
    },
};

/**
 * Battle group of the Mind-Bogglers match (BTLNO_TOU_01_06): Pale Piranha, Dark Puff, Pider, with its item
 * drop odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_14 = {
    3, // numEnemies
    btlparty_tou_tou_rank_14, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Punk Rocks match (BTLNO_TOU_01_07): H. Bald Cleft x3. */
static BattleUnitSetup btlparty_tou_tou_rank_13[] = {
    {
        &unit_hyper_sinnosuke, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinnosuke, // itemDropTable
    },
    {
        &unit_hyper_sinnosuke, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinnosuke, // itemDropTable
    },
    {
        &unit_hyper_sinnosuke, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_hyper_sinnosuke, // itemDropTable
    },
};

/**
 * Battle group of the Punk Rocks match (BTLNO_TOU_01_07): H. Bald Cleft x3, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_13 = {
    3, // numEnemies
    btlparty_tou_tou_rank_13, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Bob-omb Squad match (BTLNO_TOU_01_08): Bob-omb x4. */
static BattleUnitSetup btlparty_tou_tou_rank_12[] = {
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

/**
 * Battle group of the Bob-omb Squad match (BTLNO_TOU_01_08): Bob-omb x4, with its item drop odds and heart
 * and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_12 = {
    4, // numEnemies
    btlparty_tou_tou_rank_12, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Hand-It-Overs match (BTLNO_TOU_01_09): Bandit x2, Big Bandit x2. */
static BattleUnitSetup btlparty_tou_tou_rank_11[] = {
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo_king, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo_king, // itemDropTable
    },
    {
        &unit_borodo_king, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo_king, // itemDropTable
    },
};

/**
 * Battle group of the Hand-It-Overs match (BTLNO_TOU_01_09): Bandit x2, Big Bandit x2, with its item drop
 * odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_11 = {
    4, // numEnemies
    btlparty_tou_tou_rank_11, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Armored Harriers match (BTLNO_TOU_01_10): Iron Cleft x2. */
static BattleUnitSetup btlparty_tou_tou_rank_10[] = {
    {
        &unit_iron_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {80.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {50, 0, 0, 0}, // unitWork
        battle_item_iron_sinemon, // itemDropTable
    },
    {
        &unit_iron_sinemon2, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {130, 0, 0, 0}, // unitWork
        battle_item_iron_sinemon2, // itemDropTable
    },
};

/**
 * Battle group of the Armored Harriers match (BTLNO_TOU_01_10): Iron Cleft x2, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_10 = {
    2, // numEnemies
    btlparty_tou_tou_rank_10, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Tiny Spinies match (BTLNO_TOU_01_11): R. S. Buzzy x2. */
static BattleUnitSetup btlparty_tou_tou_rank_9[] = {
    {
        &unit_crimson_togemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togemet, // itemDropTable
    },
    {
        &unit_crimson_togemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togemet, // itemDropTable
    },
};

/**
 * Battle group of the Tiny Spinies match (BTLNO_TOU_01_11): R. S. Buzzy x2, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_9 = {
    2, // numEnemies
    btlparty_tou_tou_rank_9, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Poker Faces match (BTLNO_TOU_01_12): Bristle x2. */
static BattleUnitSetup btlparty_tou_tou_rank_8[] = {
    {
        &unit_togedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_togedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of the Poker Faces match (BTLNO_TOU_01_12): Bristle x2, with its item drop odds and heart
 * and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_8 = {
    2, // numEnemies
    btlparty_tou_tou_rank_8, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Shellshockers match (BTLNO_TOU_01_13): Shady Koopa x2, S. Paratroopa. */
static BattleUnitSetup btlparty_tou_tou_rank_7[] = {
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_ura_pata, // itemDropTable
    },
};

/**
 * Battle group of the Shellshockers match (BTLNO_TOU_01_13): Shady Koopa x2, S. Paratroopa, with its item
 * drop odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_7 = {
    3, // numEnemies
    btlparty_tou_tou_rank_7, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of The Fuzz match (BTLNO_TOU_01_14): Fuzzy, Green Fuzzy, Flower Fuzzy. */
static BattleUnitSetup btlparty_tou_tou_rank_6[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_green_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

/**
 * Battle group of The Fuzz match (BTLNO_TOU_01_14): Fuzzy, Green Fuzzy, Flower Fuzzy, with its item drop
 * odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_6 = {
    3, // numEnemies
    btlparty_tou_tou_rank_6, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of the Magikoopa Masters match (BTLNO_TOU_01_15): R. Magikoopa, W. Magikoopa, G. Magikoopa.
 */
static BattleUnitSetup btlparty_tou_tou_rank_5[] = {
    {
        &unit_kamec_red, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_kamec_white, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_fire_bros, // itemDropTable
    },
    {
        &unit_kamec_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_boomerang_bros, // itemDropTable
    },
};

/**
 * Battle group of the Magikoopa Masters match (BTLNO_TOU_01_15): R. Magikoopa, W. Magikoopa, G. Magikoopa,
 * with its item drop odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_5 = {
    3, // numEnemies
    btlparty_tou_tou_rank_5, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Craw-Daddy match (BTLNO_TOU_01_16): Dark Craw. */
static BattleUnitSetup btlparty_tou_tou_rank_4[] = {
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of the Craw-Daddy match (BTLNO_TOU_01_16): Dark Craw, with its item drop odds and heart and
 * flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_4 = {
    1, // numEnemies
    btlparty_tou_tou_rank_4, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of the Hamma, Bamma, and Flare match (BTLNO_TOU_01_17): Hammer Bro, Fire Bro, Boomerang Bro.
 */
static BattleUnitSetup btlparty_tou_tou_rank_3[] = {
    {
        &unit_hammer_bros, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_fire_bros, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_boomerang_bros, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

/**
 * Battle group of the Hamma, Bamma, and Flare match (BTLNO_TOU_01_17): Hammer Bro, Fire Bro, Boomerang Bro,
 * with its item drop odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_3 = {
    3, // numEnemies
    btlparty_tou_tou_rank_3, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Chomp Country match (BTLNO_TOU_01_18): Red Chomp x2. */
static BattleUnitSetup btlparty_tou_tou_rank_2[] = {
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

/**
 * Battle group of the Chomp Country match (BTLNO_TOU_01_18): Red Chomp x2, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_2 = {
    2, // numEnemies
    btlparty_tou_tou_rank_2, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Koopinator match (BTLNO_TOU_01_19): D. Koopatrol. */
static BattleUnitSetup btlparty_tou_tou_rank_1[] = {
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of the Koopinator match (BTLNO_TOU_01_19): D. Koopatrol, with its item drop odds and heart
 * and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_rank_1 = {
    1, // numEnemies
    btlparty_tou_tou_rank_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Wings of Night match (BTLNO_TOU_01_20): Swooper x4. */
static BattleUnitSetup btlparty_tou_tou_add_1[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Battle group of the Wings of Night match (BTLNO_TOU_01_20): Swooper x4, with its item drop odds and heart
 * and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_add_1 = {
    4, // numEnemies
    btlparty_tou_tou_add_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the Destructors match (BTLNO_TOU_01_21): Spinia x2, Spania x2. */
static BattleUnitSetup btlparty_tou_tou_add_2[] = {
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of the Destructors match (BTLNO_TOU_01_21): Spinia x2, Spania x2, with its item drop odds
 * and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_add_2 = {
    4, // numEnemies
    btlparty_tou_tou_add_2, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the boss fight against Macho Grubba (BTLNO_TOU_01_23): Macho Grubba. */
static BattleUnitSetup btlparty_tou_tou_boss[] = {
    {
        &unit_boss_macho_gance, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_macho_gance, // itemDropTable
    },
};

/**
 * Battle group of the boss fight against Macho Grubba (BTLNO_TOU_01_23): Macho Grubba, with its item drop
 * odds and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_boss = {
    1, // numEnemies
    btlparty_tou_tou_boss, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of the title match against Rawk Hawk (BTLNO_TOU_01_22): Rawk Hawk. */
static BattleUnitSetup btlparty_tou_tou_champ[] = {
    {
        &unit_boss_champion, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_champion, // itemDropTable
    },
};

/**
 * Battle group of the title match against Rawk Hawk (BTLNO_TOU_01_22): Rawk Hawk, with its item drop odds
 * and heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_champ = {
    1, // numEnemies
    btlparty_tou_tou_champ, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of Bowser's surprise intrusion fight (BTLNO_TOU_01_24): Bowser. */
static BattleUnitSetup btlparty_tou_tou_koopa[] = {
    {
        &unit_boss_koopa_tou, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Battle group of Bowser's surprise intrusion fight (BTLNO_TOU_01_24): Bowser, with its item drop odds and
 * heart and flower drop tables.
 */
static BattleGroupSetup btlgrp_tou_tou_koopa = {
    1, // numEnemies
    btlparty_tou_tou_koopa, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Debug battle choice BTLNO_TOU_TEST1: reuses the Goomba Bros. group on the normal arena. */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_test1_off[] = {
    {50, &btlgrp_tou_tou_rank_19, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Goomba Bros. match (BTLNO_TOU_01_01): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_01_off[] = {
    {50, &btlgrp_tou_tou_rank_19, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the KP Koopas match (BTLNO_TOU_01_02): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_02_off[] = {
    {50, &btlgrp_tou_tou_rank_18, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Pokey Triplets match (BTLNO_TOU_01_03): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_03_off[] = {
    {50, &btlgrp_tou_tou_rank_17, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Dead Bones match (BTLNO_TOU_01_04): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_04_off[] = {
    {50, &btlgrp_tou_tou_rank_16, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Spike Storm match (BTLNO_TOU_01_05): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_05_off[] = {
    {50, &btlgrp_tou_tou_rank_15, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Mind-Bogglers match (BTLNO_TOU_01_06): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_06_off[] = {
    {50, &btlgrp_tou_tou_rank_14, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Punk Rocks match (BTLNO_TOU_01_07): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_07_off[] = {
    {50, &btlgrp_tou_tou_rank_13, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Bob-omb Squad match (BTLNO_TOU_01_08): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_08_off[] = {
    {50, &btlgrp_tou_tou_rank_12, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Hand-It-Overs match (BTLNO_TOU_01_09): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_09_off[] = {
    {50, &btlgrp_tou_tou_rank_11, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Armored Harriers match (BTLNO_TOU_01_10): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_10_off[] = {
    {50, &btlgrp_tou_tou_rank_10, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Tiny Spinies match (BTLNO_TOU_01_11): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_11_off[] = {
    {50, &btlgrp_tou_tou_rank_9, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Poker Faces match (BTLNO_TOU_01_12): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_12_off[] = {
    {50, &btlgrp_tou_tou_rank_8, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Shellshockers match (BTLNO_TOU_01_13): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_13_off[] = {
    {50, &btlgrp_tou_tou_rank_7, tou_btlstage_data},
    {0},
};

/**
 * Group choice for The Fuzz match (BTLNO_TOU_01_14): always the one listed group (weight 50) on the normal
 * arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_14_off[] = {
    {50, &btlgrp_tou_tou_rank_6, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Magikoopa Masters match (BTLNO_TOU_01_15): always the one listed group (weight 50)
 * on the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_15_off[] = {
    {50, &btlgrp_tou_tou_rank_5, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Craw-Daddy match (BTLNO_TOU_01_16): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_16_off[] = {
    {50, &btlgrp_tou_tou_rank_4, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Hamma, Bamma, and Flare match (BTLNO_TOU_01_17): always the one listed group (weight
 * 50) on the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_17_off[] = {
    {50, &btlgrp_tou_tou_rank_3, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Chomp Country match (BTLNO_TOU_01_18): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_18_off[] = {
    {50, &btlgrp_tou_tou_rank_2, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Koopinator match (BTLNO_TOU_01_19): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_19_off[] = {
    {50, &btlgrp_tou_tou_rank_1, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Wings of Night match (BTLNO_TOU_01_20): always the one listed group (weight 50) on
 * the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_20_off[] = {
    {50, &btlgrp_tou_tou_add_1, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the Destructors match (BTLNO_TOU_01_21): always the one listed group (weight 50) on the
 * normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_21_off[] = {
    {50, &btlgrp_tou_tou_add_2, tou_btlstage_data},
    {0},
};

/**
 * Group choice for the title match against Rawk Hawk (BTLNO_TOU_01_22): always the one listed group (weight
 * 50) on the title-match arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_22_off[] = {
    {50, &btlgrp_tou_tou_champ, &tou_btlstage_data[1]},
    {0},
};

/**
 * Group choice for the boss fight against Macho Grubba (BTLNO_TOU_01_23): always the one listed group
 * (weight 50) on the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_23_off[] = {
    {50, &btlgrp_tou_tou_boss, tou_btlstage_data},
    {0},
};

/**
 * Group choice for Bowser's surprise intrusion fight (BTLNO_TOU_01_24): always the one listed group (weight
 * 50) on the normal arena stage.
 */
static NpcBattleSelect btlgrpsel_tou_btlno_tou_01_24_off[] = {
    {50, &btlgrp_tou_tou_koopa, tou_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_tou_tbl[] = {
    {
        "マップstg_03_0", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "クリクリブラザーズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_01_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "コウラーズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_02_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "サンボ三兄弟", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_03_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ボンバーズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_04_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "クモーンズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_05_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ジュゲムとイタい仲間たち", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_06_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ハイパーズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_07_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ボロドーズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_08_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "クッパぐんだんズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_09_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "こうてつウォリアーズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_10_off, // select
        (NPC_BATTLE_NO_GAME_OVER | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        "ザ・トゲトゲン", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_11_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミトゲダルマー", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_12_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ザ・ウラウラン", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_13_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "レッドデビルズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_14_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "トリプルブロス", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_15_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ザ・マホーンズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_16_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "キラーボムヘイズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_17_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バーストワンワン", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_18_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "トゲノコエース", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_19_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ナイトウィングス", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_20_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "スネイルズ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_21_off, // select
        NPC_BATTLE_NO_GAME_OVER, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ブレッドハート", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_22_off, // select
        (NPC_BATTLE_NO_GAME_OVER | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG3_CHAMP1", // bgmName
    },
    {
        "マッチョガンス", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_23_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        6, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG3_GANCE1", // bgmName
    },
    {
        "乱入クッパ", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_24_off, // select
#if VERSION >= VERSION_US
        (NPC_BATTLE_NO_GAME_OVER | NPC_BATTLE_STORY), // battleFlags
#else
        NPC_BATTLE_STORY, // battleFlags
#endif
        0, // field_1C
        {{0}, {0}, {0}, {0}, {4, 4}, {4, 4}, {0}, {0}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG8_KOOPA", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_tou_setup_no_tbl[] = {
    {"BTLNO_TOU_TEST1", 0},
    {"BTLNO_TOU_01_01", 1},
    {"BTLNO_TOU_01_02", 2},
    {"BTLNO_TOU_01_03", 3},
    {"BTLNO_TOU_01_04", 4},
    {"BTLNO_TOU_01_05", 5},
    {"BTLNO_TOU_01_06", 6},
    {"BTLNO_TOU_01_07", 7},
    {"BTLNO_TOU_01_08", 8},
    {"BTLNO_TOU_01_09", 9},
    {"BTLNO_TOU_01_10", 10},
    {"BTLNO_TOU_01_11", 11},
    {"BTLNO_TOU_01_12", 12},
    {"BTLNO_TOU_01_13", 13},
    {"BTLNO_TOU_01_14", 14},
    {"BTLNO_TOU_01_15", 15},
    {"BTLNO_TOU_01_16", 16},
    {"BTLNO_TOU_01_17", 17},
    {"BTLNO_TOU_01_18", 18},
    {"BTLNO_TOU_01_19", 19},
    {"BTLNO_TOU_01_20", 20},
    {"BTLNO_TOU_01_21", 21},
    {"BTLNO_TOU_01_22", 22},
    {"BTLNO_TOU_01_23", 23},
    {"BTLNO_TOU_01_24", 24},
    {0},
};

/**
 * Battle stage background script for the Glitz Pit arenas:
 * initialises the big monitor texture, then installs tou_gamen_screen_tev as TEV callback 8 on the "gamen"
 * (screen) map object so the monitor shows the battle picture.
 */
EVT_BEGIN(bgset_tou_screen_evt)
    CALL(tou_gamen_screen_tev_init)
    WAIT_FRAMES(1)
    CALL(evt_map_set_tevcallback, 8, PTR(tou_gamen_screen_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("gamen"), MAP_OBJ_TEV_CALLBACK(8))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("gamen"), MAP_OBJ_FLAG_DISPLAY_LIST)
    RETURN
EVT_END()
