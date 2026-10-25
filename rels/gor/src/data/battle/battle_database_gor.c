#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "gor.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_boss_kanbu1;
extern BattleUnitKind unit_lecture_christine;

BattleStageObjectData gor_btlobject_data[6];
BattleStageData gor_btlstage_data[2];
BattleUnitSetup btlparty_gor_gor_00_01_off_1[1];
BattleGroupSetup btlgrp_gor_gor_00_01_off_1;
BattleUnitSetup btlparty_gor_gor_02_ac_lecture[1];
BattleGroupSetup btlgrp_gor_gor_02_ac_lecture;
BattleUnitSetup btlparty_gor_gor_02_sac_lecture[1];
BattleGroupSetup btlgrp_gor_gor_02_sac_lecture;
BattleUnitSetup btlparty_gor_gor_02_01_off_1[1];
BattleGroupSetup btlgrp_gor_gor_02_01_off_1;
NpcBattleSelect btlgrpsel_gor_btlno_gor_test1_off[2];
NpcBattleSelect btlgrpsel_gor_btlno_gor_test2_off[2];
NpcBattleSelect btlgrpsel_gor_btlno_gor_00_01_off[2];
NpcBattleSelect btlgrpsel_gor_btlno_gor_02_aclec_off[2];
NpcBattleSelect btlgrpsel_gor_btlno_gor_02_saclec_off[2];
NpcBattleSelect btlgrpsel_gor_btlno_gor_02_01_off[2];
NpcSetupBattleInfo btlsetup_gor_tbl[7];
NpcTerritoryInfo btl_gor_setup_no_tbl[7];

BattleStageObjectData gor_btlobject_data[] = {
    {"A1", 2, 0, {33.0f, 0.0f, -48.0f}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, 71.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData gor_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_0", // mSpecificStageDataDir
        4, // mNumObjects
        gor_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100070, 0x10000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_4", // mSpecificStageDataDir
        2, // mNumObjects
        &gor_btlobject_data[4], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_gor_gor_00_01_off_1[] = {
    {
        &unit_boss_kanbu1, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kanbu1, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gor_gor_00_01_off_1 = {
    1, // numEnemies
    btlparty_gor_gor_00_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gor_gor_02_ac_lecture[] = {
    {
        &unit_lecture_christine, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gor_gor_02_ac_lecture = {
    1, // numEnemies
    btlparty_gor_gor_02_ac_lecture, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gor_gor_02_sac_lecture[] = {
    {
        &unit_lecture_frankli_sac, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gor_gor_02_sac_lecture = {
    1, // numEnemies
    btlparty_gor_gor_02_sac_lecture, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gor_gor_02_01_off_1[] = {
    {
        &unit_monban, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gor_gor_02_01_off_1 = {
    1, // numEnemies
    btlparty_gor_gor_02_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_gor_btlno_gor_test1_off[] = {
    {50, &btlgrp_gor_gor_02_01_off_1, gor_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gor_btlno_gor_test2_off[] = {
    {50, &btlgrp_gor_gor_02_01_off_1, &gor_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gor_btlno_gor_00_01_off[] = {
    {50, &btlgrp_gor_gor_00_01_off_1, gor_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gor_btlno_gor_02_aclec_off[] = {
    {50, &btlgrp_gor_gor_02_ac_lecture, &gor_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gor_btlno_gor_02_saclec_off[] = {
    {50, &btlgrp_gor_gor_02_sac_lecture, &gor_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gor_btlno_gor_02_01_off[] = {
    {50, &btlgrp_gor_gor_02_01_off_1, &gor_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_gor_tbl[] = {
    {
        /*"マップstg_00_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x30\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gor_btlno_gor_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_00_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x30\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gor_btlno_gor_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GOR00 幹部その１"*/"\x47\x4F\x52\x30\x30\x20\x8A\xB2\x95\x94\x82\xBB\x82\xCC\x82\x50", // name
        "gor_00", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gor_btlno_gor_00_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{0}, {5, 5}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"GOR02 ACレクチャー"*/"\x47\x4F\x52\x30\x32\x20\x41\x43\x83\x8C\x83\x4E\x83\x60\x83\x83\x81\x5B", // name
        "gor_02", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gor_btlno_gor_02_aclec_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        /*"GOR02 SACレクチャー"*/"\x47\x4F\x52\x30\x32\x20\x53\x41\x43\x83\x8C\x83\x4E\x83\x60\x83\x83\x81\x5B", // name
        "gor_02", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gor_btlno_gor_02_saclec_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        /*"GOR02 もんばん"*/"\x47\x4F\x52\x30\x32\x20\x82\xE0\x82\xF1\x82\xCE\x82\xF1", // name
        "gor_02", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gor_btlno_gor_02_01_off, // select
        0x20000000, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_gor_setup_no_tbl[] = {
    {"BTLNO_GOR_TEST1", 0},
    {"BTLNO_GOR_TEST2", 1},
    {"BTLNO_GOR_00_01", 2},
    {"BTLNO_GOR_02_ACLEC", 3},
    {"BTLNO_GOR_02_SACLEC", 4},
    {"BTLNO_GOR_02_01", 5},
    {0},
};

