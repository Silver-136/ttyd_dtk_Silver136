#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_act_mario;

BattleStageObjectData nok_btlobject_data[10];
BattleStageData nok_btlstage_data[1];
BattleUnitSetup btlparty_nok_hei_ep[1];
BattleGroupSetup btlgrp_nok_hei_ep;
NpcBattleSelect btlgrpsel_nok_btlno_hei_test1_off[2];
NpcSetupBattleInfo btlsetup_nok_tbl[2];
NpcTerritoryInfo btl_nok_setup_no_tbl[2];

BattleStageObjectData nok_btlobject_data[] = {
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
};

BattleStageData nok_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_0", // mSpecificStageDataDir
        10, // mNumObjects
        nok_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_nok_hei_ep[] = {
    {
        &unit_act_mario, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_nok_hei_ep = {
    1, // numEnemies
    btlparty_nok_hei_ep, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_nok_btlno_hei_test1_off[] = {
    {50, &btlgrp_nok_hei_ep, nok_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_nok_tbl[] = {
    {
        /*"マップstg_01_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x30", // name
        "nok_01", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_nok_btlno_hei_test1_off, // select
        0x100000, // battleFlags
        6, // field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_nok_setup_no_tbl[] = {
    {"BTLNO_HEI_TEST1", 0},
    {0},
};

