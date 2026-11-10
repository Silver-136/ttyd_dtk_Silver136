// Battle database of the Moon (moo.rel): stage data, enemy groups and the encounter tables of its maps.

#include "version.h"
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

static BattleStageObjectData moo_btlobject_data[5];
static BattleStageData moo_btlstage_data[1];
static BattleUnitSetup btlparty_moo_moo_00_01_off_1[3];
static BattleGroupSetup btlgrp_moo_moo_00_01_off_1;
static BattleUnitSetup btlparty_moo_moo_01_01_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_01_01_off_1;
static BattleUnitSetup btlparty_moo_moo_01_02_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_01_02_off_1;
static BattleUnitSetup btlparty_moo_moo_01_02_off_2[3];
static BattleGroupSetup btlgrp_moo_moo_01_02_off_2;
static BattleUnitSetup btlparty_moo_moo_01_03_off_1[3];
static BattleGroupSetup btlgrp_moo_moo_01_03_off_1;
static BattleUnitSetup btlparty_moo_moo_01_04_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_01_04_off_1;
static BattleUnitSetup btlparty_moo_moo_01_04_off_2[3];
static BattleGroupSetup btlgrp_moo_moo_01_04_off_2;
static BattleUnitSetup btlparty_moo_moo_01_04_off_3[4];
static BattleGroupSetup btlgrp_moo_moo_01_04_off_3;
static BattleUnitSetup btlparty_moo_moo_01_05_off_1[4];
static BattleGroupSetup btlgrp_moo_moo_01_05_off_1;
static BattleUnitSetup btlparty_moo_moo_02_01_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_02_01_off_1;
static BattleUnitSetup btlparty_moo_moo_02_02_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_02_02_off_1;
static BattleUnitSetup btlparty_moo_moo_02_03_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_02_03_off_1;
static BattleUnitSetup btlparty_moo_moo_02_04_off_1[3];
static BattleGroupSetup btlgrp_moo_moo_02_04_off_1;
static BattleUnitSetup btlparty_moo_moo_02_05_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_02_05_off_1;
static BattleUnitSetup btlparty_moo_moo_05_01_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_05_01_off_1;
static BattleUnitSetup btlparty_moo_moo_05_01_off_2[3];
static BattleGroupSetup btlgrp_moo_moo_05_01_off_2;
static BattleUnitSetup btlparty_moo_moo_05_01_off_3[4];
static BattleGroupSetup btlgrp_moo_moo_05_01_off_3;
static BattleUnitSetup btlparty_moo_moo_05_02_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_05_02_off_1;
static BattleUnitSetup btlparty_moo_moo_05_03_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_05_03_off_1;
static BattleUnitSetup btlparty_moo_moo_05_03_off_2[3];
static BattleGroupSetup btlgrp_moo_moo_05_03_off_2;
static BattleUnitSetup btlparty_moo_moo_05_04_off_1[3];
static BattleGroupSetup btlgrp_moo_moo_05_04_off_1;
static BattleUnitSetup btlparty_moo_moo_05_05_off_1[2];
static BattleGroupSetup btlgrp_moo_moo_05_05_off_1;
static BattleUnitSetup btlparty_moo_moo_05_05_off_2[3];
static BattleGroupSetup btlgrp_moo_moo_05_05_off_2;
static BattleUnitSetup btlparty_moo_moo_05_05_off_3[4];
static BattleGroupSetup btlgrp_moo_moo_05_05_off_3;
static NpcBattleSelect btlgrpsel_moo_btlno_moo_test1_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_01_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_02_off[3];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_03_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_04_off[4];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_05_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_01_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_02_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_03_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_04_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_05_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_01_off[4];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_02_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_03_off[3];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_04_off[2];
static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_05_off[4];
NpcSetupBattleInfo btlsetup_moo_tbl[17];
NpcTerritoryInfo btl_moo_setup_no_tbl[17];
static EvtWord bgset_moo_space_event[35];

/** Scenery pieces of the Moon battle stage; moo_btlstage_data points into this array. */
static BattleStageObjectData moo_btlobject_data[] = {
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"space", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"star_01", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"star_02", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"star_03", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
};

/** The battle stage used for fights on the Moon: its backdrop folder and scenery pieces. */
static BattleStageData moo_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_07_4", // specificStageDataDir
        5, // numObjects
        moo_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_moo_space_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

static BattleUnitSetup btlparty_moo_moo_00_01_off_1[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
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

static BattleGroupSetup btlgrp_moo_moo_00_01_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_01_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_01_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_02_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_02_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_01_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_02_off_2[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_02_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_01_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_03_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_03_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_04_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_04_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_01_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_04_off_2[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_04_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_01_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_04_off_3[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_04_off_3 = {
    4, // numEnemies
    btlparty_moo_moo_01_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_01_05_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_01_05_off_1 = {
    4, // numEnemies
    btlparty_moo_moo_01_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_02_01_off_1[] = {
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {70.0f, 40.0f, 0.0f},
#else
        {50.0f, 40.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {110.0f, 40.0f, 10.0f},
#else
        {130.0f, 40.0f, 10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_02_01_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_02_02_off_1[] = {
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_02_02_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_02_03_off_1[] = {
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {70.0f, 40.0f, 0.0f},
#else
        {50.0f, 40.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {110.0f, 40.0f, 10.0f},
#else
        {130.0f, 40.0f, 10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_02_03_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_02_04_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_02_04_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_02_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_02_05_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_02_05_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_01_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_01_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_01_off_2[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_01_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_05_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_01_off_3[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_01_off_3 = {
    4, // numEnemies
    btlparty_moo_moo_05_01_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_02_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_02_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_03_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_03_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_03_off_2[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_03_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_05_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_04_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_04_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_05_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_05_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_05_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_05_off_2[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_05_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_05_05_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static BattleUnitSetup btlparty_moo_moo_05_05_off_3[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

static BattleGroupSetup btlgrp_moo_moo_05_05_off_3 = {
    4, // numEnemies
    btlparty_moo_moo_05_05_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_test1_off[] = {
    {50, &btlgrp_moo_moo_00_01_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_01_off[] = {
    {10, &btlgrp_moo_moo_01_01_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_02_off[] = {
    {10, &btlgrp_moo_moo_01_02_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_01_02_off_2, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_03_off[] = {
    {10, &btlgrp_moo_moo_01_03_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_04_off[] = {
    {10, &btlgrp_moo_moo_01_04_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_01_04_off_2, moo_btlstage_data},
    {5, &btlgrp_moo_moo_01_04_off_3, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_01_05_off[] = {
    {5, &btlgrp_moo_moo_01_05_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_01_off[] = {
    {20, &btlgrp_moo_moo_02_01_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_02_off[] = {
    {20, &btlgrp_moo_moo_02_02_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_03_off[] = {
    {20, &btlgrp_moo_moo_02_03_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_04_off[] = {
    {20, &btlgrp_moo_moo_02_04_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_02_05_off[] = {
    {20, &btlgrp_moo_moo_02_05_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_01_off[] = {
    {10, &btlgrp_moo_moo_05_01_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_05_01_off_2, moo_btlstage_data},
    {5, &btlgrp_moo_moo_05_01_off_3, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_02_off[] = {
    {20, &btlgrp_moo_moo_05_02_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_03_off[] = {
    {10, &btlgrp_moo_moo_05_03_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_05_03_off_2, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_04_off[] = {
    {20, &btlgrp_moo_moo_05_04_off_1, moo_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_moo_btlno_moo_05_05_off[] = {
    {10, &btlgrp_moo_moo_05_05_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_05_05_off_2, moo_btlstage_data},
    {5, &btlgrp_moo_moo_05_05_off_3, moo_btlstage_data},
    {0},
};

/**
 * Encounter settings of every Moon map: which battle choice its enemies use, the battle music and the random
 * audience rolls.
 */
NpcSetupBattleInfo btlsetup_moo_tbl[] = {
    {
        "マップstg_07_4", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo01シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo01シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo01シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo01シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_04_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo01シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_05_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo02バリアーンZ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo02バリアーンZ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo02バリアーンZ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo02シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_04_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo02シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_05_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo05シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo05シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo05シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo05シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_04_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "moo05シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_05_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
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

/**
 * Event script: the animated space background of the Moon battles.
 * It rotates the three star objects of the stage (star_01, star_02, star_03) about Z once every 7500,
 * 6750 and 6000 frames, forever.
 */
static EVT_BEGIN(bgset_moo_space_event)
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

/**
 * Script helper: turns the stage object named in args[0] around Z. args[1] is a script variable holding the
 * current step, args[2] the number of steps in one turn:
 * it sets the rotation to 360 degrees times step/total and advances and wraps the step.
 */
static USER_FUNC(_space_rotate) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 outVar = args[1];
    s32 step = evtGetValue(event, outVar);
    s32 total = evtGetValue(event, args[2]);
    BattleStageObject* obj = BattleSearchObjectPtr(name);

    obj->rotation.z = 360.0f * step / total;
    step++;
    if (step >= total) {
        step = 0;
    }
    evtSetValue(event, outVar, step);
    return EVT_RETURN_DONE;
}
