#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_sub.h"

#include "battle/battle_stage_object.h"
#include "system.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "dou.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"


static USER_FUNC(_wave_offset);
extern BattleUnitKind unit_killer_cannon;

BattleStageObjectData dou_btlobject_data[51];
BattleStageData dou_btlstage_data[6];
BattleUnitSetup btlparty_dou_dou_00_01_off_1[3];
BattleGroupSetup btlgrp_dou_dou_00_01_off_1;
BattleUnitSetup btlparty_dou_dou_01_01_off_01[3];
BattleGroupSetup btlgrp_dou_dou_01_01_off_01;
BattleUnitSetup btlparty_dou_dou_02_01_off_01[2];
BattleGroupSetup btlgrp_dou_dou_02_01_off_01;
BattleUnitSetup btlparty_dou_dou_02_02_off_01[4];
BattleGroupSetup btlgrp_dou_dou_02_02_off_01;
BattleUnitSetup btlparty_dou_dou_03_01_off_01[2];
BattleGroupSetup btlgrp_dou_dou_03_01_off_01;
BattleUnitSetup btlparty_dou_dou_03_99_off_01[2];
BattleGroupSetup btlgrp_dou_dou_03_99_off_01;
BattleUnitSetup btlparty_dou_dou_03_02_off_01[2];
BattleGroupSetup btlgrp_dou_dou_03_02_off_01;
BattleUnitSetup btlparty_dou_dou_03_03_off_01[3];
BattleGroupSetup btlgrp_dou_dou_03_03_off_01;
BattleUnitSetup btlparty_dou_dou_05_01_off_01[2];
BattleGroupSetup btlgrp_dou_dou_05_01_off_01;
BattleUnitSetup btlparty_dou_dou_05_02_off_01[2];
BattleGroupSetup btlgrp_dou_dou_05_02_off_01;
BattleUnitSetup btlparty_dou_dou_06_01_off_01[3];
BattleGroupSetup btlgrp_dou_dou_06_01_off_01;
BattleUnitSetup btlparty_dou_dou_06_02_off_01[4];
BattleGroupSetup btlgrp_dou_dou_06_02_off_01;
BattleUnitSetup btlparty_dou_dou_06_03_off_01[1];
BattleGroupSetup btlgrp_dou_dou_06_03_off_01;
BattleUnitSetup btlparty_dou_dou_07_01_off_01[3];
BattleGroupSetup btlgrp_dou_dou_07_01_off_01;
BattleUnitSetup btlparty_dou_dou_08_01_off_01[4];
BattleGroupSetup btlgrp_dou_dou_08_01_off_01;
BattleUnitSetup btlparty_dou_dou_09_01_off_01[3];
BattleGroupSetup btlgrp_dou_dou_09_01_off_01;
BattleUnitSetup btlparty_dou_dou_09_02_off_01[3];
BattleGroupSetup btlgrp_dou_dou_09_02_off_01;
BattleUnitSetup btlparty_dou_dou_12_01_off_01[2];
BattleGroupSetup btlgrp_dou_dou_12_01_off_01;
BattleUnitSetup btlparty_dou_dou_12_99_off_01[2];
BattleGroupSetup btlgrp_dou_dou_12_99_off_01;
BattleUnitSetup btlparty_dou_dou_13_01_off_01[2];
BattleGroupSetup btlgrp_dou_dou_13_01_off_01;
BattleUnitSetup btlparty_dou_dou_13_99_off_01[2];
BattleGroupSetup btlgrp_dou_dou_13_99_off_01;
NpcBattleSelect btlgrpsel_dou_btlno_dou_test1_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_test2_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_test3_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_01_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_02_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_02_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_03_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_03_99_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_03_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_03_03_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_05_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_05_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_06_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_06_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_06_03_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_07_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_08_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_09_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_09_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_12_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_12_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_12_99_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_13_01_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_13_02_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_13_03_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_13_04_off[2];
NpcBattleSelect btlgrpsel_dou_btlno_dou_13_99_off[2];
NpcSetupBattleInfo btlsetup_dou_tbl[28];
NpcTerritoryInfo btl_dou_setup_no_tbl[28];
EvtWord bgset_dou_wave_event[77];

BattleStageObjectData dou_btlobject_data[] = {
    {"taki", 2, 2, {0}, 0, 60, {0}},
    {"B_kabe", 2, 2, {0}, 10, 50, {0}},
    {"zabu_1", 2, 2, {0}, 20, 40, {0}},
    {"zabu_2", 2, 2, {0}, 30, 30, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B_kabe", 2, 2, {0}, 10, 50, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"takara_01", 2, 1, {111.0f, 0.0f, -69.0f}, 0, 60, {0}},
    {"takara_02", 2, 1, {-87.0f, 0.0f, -72.0f}, 10, 50, {0}},
    {"takara_03", 2, 1, {136.0f, 0.0f, 57.0f}, 20, 40, {0}},
    {"takara_04", 2, 1, {-182.0f, 0.0f, 57.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C_kabe", 2, 3, {0}, 0, 60, {0}},
    {"nami_00", 2, 3, {0}, 0, 60, {0}},
    {"nami_01", 2, 3, {0}, 0, 60, {0}},
    {"fune_L", 2, 0, {-104.0f, 0.0f, -64.0f}, 0, 60, {0}},
    {"fune_R", 2, 0, {120.0f, 0.0f, -64.0f}, 10, 50, {0}},
    {"A1_kabe", 2, 0, {0.0f, 0.0f, -51.0f}, 20, 40, {0}},
    {"iwa_01", 2, 1, {232.0f, 0.0f, -21.0f}, 0, 60, {0}},
    {"iwa_02", 2, 1, {-234.0f, 0.0f, -21.0f}, 10, 50, {0}},
    {"iwa_03", 2, 1, {0.0f, 0.0f, -38.0f}, 20, 40, {0}},
    {"B_kabe", 2, 2, {0}, 0, 60, {0}},
    {"nami_02", 2, 2, {0}, 10, 50, {0}},
    {"nami_03", 2, 2, {0}, 20, 40, {0}},
    {"nami_04", 2, 2, {0}, 30, 30, {0}},
    {"C_kabe", 2, 3, {0}, 0, 60, {0}},
    {"nami_00", 2, 3, {0}, 0, 60, {0}},
    {"nami_01", 2, 3, {0}, 0, 60, {0}},
    {"fune_R", 2, 0, {120.0f, 0.0f, -64.0f}, 10, 50, {0}},
    {"A1_kabe", 2, 0, {0.0f, 0.0f, -51.0f}, 20, 40, {0}},
    {"iwa_01", 2, 1, {232.0f, 0.0f, -21.0f}, 0, 60, {0}},
    {"iwa_02", 2, 1, {-234.0f, 0.0f, -21.0f}, 10, 50, {0}},
    {"iwa_03", 2, 1, {0.0f, 0.0f, -38.0f}, 20, 40, {0}},
    {"B_kabe", 2, 2, {0}, 0, 60, {0}},
    {"nami_02", 2, 2, {0}, 10, 50, {0}},
    {"nami_03", 2, 2, {0}, 20, 40, {0}},
    {"nami_04", 2, 2, {0}, 30, 30, {0}},
    {"C_kabe", 2, 3, {0}, 0, 60, {0}},
    {"nami_00", 2, 3, {0}, 0, 60, {0}},
    {"nami_01", 2, 3, {0}, 0, 60, {0}},
    {"A1_kabe", 2, 0, {0.0f, 0.0f, -51.0f}, 20, 40, {0}},
    {"iwa_01", 2, 1, {192.0f, 0.0f, 62.0f}, 0, 60, {0}},
    {"iwa_02", 2, 1, {-173.0f, 0.0f, 53.0f}, 10, 50, {0}},
    {"iwa_03", 2, 1, {0.0f, 0.0f, -38.0f}, 20, 40, {0}},
    {"B_kabe", 2, 2, {0}, 0, 60, {0}},
    {"nami_02", 2, 2, {0}, 10, 50, {0}},
    {"nami_03", 2, 2, {0}, 20, 40, {0}},
    {"nami_04", 2, 2, {0}, 30, 30, {0}},
    {"C_kabe", 2, 3, {0}, 0, 60, {0}},
    {"nami_00", 2, 3, {0}, 0, 60, {0}},
    {"nami_01", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData dou_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_4", // mSpecificStageDataDir
        5, // mNumObjects
        dou_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_4", // mSpecificStageDataDir
        2, // mNumObjects
        &dou_btlobject_data[5], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_5", // mSpecificStageDataDir
        8, // mNumObjects
        &dou_btlobject_data[7], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_3", // mSpecificStageDataDir
        13, // mNumObjects
        &dou_btlobject_data[15], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgA2Evt
        bgset_dou_wave_event, // bgBEvt
        NULL, 0, // bgCEvt..reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_3", // mSpecificStageDataDir
        12, // mNumObjects
        &dou_btlobject_data[28], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgA2Evt
        bgset_dou_wave_event, // bgBEvt
        NULL, 0, // bgCEvt..reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_3", // mSpecificStageDataDir
        11, // mNumObjects
        &dou_btlobject_data[40], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgA2Evt
        bgset_dou_wave_event, // bgBEvt
        NULL, 0, // bgCEvt..reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
};

BattleUnitSetup btlparty_dou_dou_00_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_00_01_off_1 = {
    3, // numEnemies
    btlparty_dou_dou_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_01_01_off_01[] = {
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_01_01_off_01 = {
    3, // numEnemies
    btlparty_dou_dou_01_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_02_01_off_01[] = {
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_02_01_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_02_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_02_02_off_01[] = {
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_02_02_off_01 = {
    4, // numEnemies
    btlparty_dou_dou_02_02_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_03_01_off_01[] = {
    {
        &unit_killer_cannon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer_cannon, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_03_01_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_03_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_03_99_off_01[] = {
    {
        &unit_killer, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer, // itemDropTable
    },
    {
        &unit_killer, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_03_99_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_03_99_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_03_02_off_01[] = {
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_03_02_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_03_02_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_03_03_off_01[] = {
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_03_03_off_01 = {
    3, // numEnemies
    btlparty_dou_dou_03_03_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_05_01_off_01[] = {
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_05_01_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_05_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_05_02_off_01[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_05_02_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_05_02_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_06_01_off_01[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_06_01_off_01 = {
    3, // numEnemies
    btlparty_dou_dou_06_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_06_02_off_01[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_06_02_off_01 = {
    4, // numEnemies
    btlparty_dou_dou_06_02_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_06_03_off_01[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_06_03_off_01 = {
    1, // numEnemies
    btlparty_dou_dou_06_03_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_07_01_off_01[] = {
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_07_01_off_01 = {
    3, // numEnemies
    btlparty_dou_dou_07_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_08_01_off_01[] = {
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_08_01_off_01 = {
    4, // numEnemies
    btlparty_dou_dou_08_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_09_01_off_01[] = {
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_09_01_off_01 = {
    3, // numEnemies
    btlparty_dou_dou_09_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_09_02_off_01[] = {
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hermos, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_09_02_off_01 = {
    3, // numEnemies
    btlparty_dou_dou_09_02_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_12_01_off_01[] = {
    {
        &unit_killer_cannon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer_cannon, // itemDropTable
    },
    {
        &unit_killer_cannon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_12_01_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_12_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_12_99_off_01[] = {
    {
        &unit_killer, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer, // itemDropTable
    },
    {
        &unit_killer, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_12_99_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_12_99_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_13_01_off_01[] = {
    {
        &unit_killer_cannon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer_cannon, // itemDropTable
    },
    {
        &unit_killer_cannon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_13_01_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_13_01_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_dou_dou_13_99_off_01[] = {
    {
        &unit_killer, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer, // itemDropTable
    },
    {
        &unit_killer, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_killer, // itemDropTable
    },
};

BattleGroupSetup btlgrp_dou_dou_13_99_off_01 = {
    2, // numEnemies
    btlparty_dou_dou_13_99_off_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_test1_off[] = {
    {50, &btlgrp_dou_dou_00_01_off_1, &dou_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_test2_off[] = {
    {50, &btlgrp_dou_dou_00_01_off_1, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_test3_off[] = {
    {50, &btlgrp_dou_dou_00_01_off_1, &dou_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_01_01_off[] = {
    {20, &btlgrp_dou_dou_01_01_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_02_01_off[] = {
    {20, &btlgrp_dou_dou_02_01_off_01, &dou_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_02_02_off[] = {
    {20, &btlgrp_dou_dou_02_02_off_01, &dou_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_03_01_off[] = {
    {20, &btlgrp_dou_dou_03_01_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_03_99_off[] = {
    {20, &btlgrp_dou_dou_03_99_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_03_02_off[] = {
    {20, &btlgrp_dou_dou_03_02_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_03_03_off[] = {
    {20, &btlgrp_dou_dou_03_03_off_01, &dou_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_05_01_off[] = {
    {20, &btlgrp_dou_dou_05_01_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_05_02_off[] = {
    {20, &btlgrp_dou_dou_05_02_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_06_01_off[] = {
    {20, &btlgrp_dou_dou_06_01_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_06_02_off[] = {
    {20, &btlgrp_dou_dou_06_02_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_06_03_off[] = {
    {20, &btlgrp_dou_dou_06_03_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_07_01_off[] = {
    {20, &btlgrp_dou_dou_07_01_off_01, dou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_08_01_off[] = {
    {20, &btlgrp_dou_dou_08_01_off_01, &dou_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_09_01_off[] = {
    {20, &btlgrp_dou_dou_09_01_off_01, &dou_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_09_02_off[] = {
    {20, &btlgrp_dou_dou_09_02_off_01, &dou_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_12_01_off[] = {
    {20, &btlgrp_dou_dou_12_01_off_01, &dou_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_12_02_off[] = {
    {20, &btlgrp_dou_dou_12_01_off_01, &dou_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_12_99_off[] = {
    {20, &btlgrp_dou_dou_12_99_off_01, &dou_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_13_01_off[] = {
    {20, &btlgrp_dou_dou_13_01_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_13_02_off[] = {
    {20, &btlgrp_dou_dou_13_01_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_13_03_off[] = {
    {20, &btlgrp_dou_dou_13_01_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_13_04_off[] = {
    {20, &btlgrp_dou_dou_13_01_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_dou_btlno_dou_13_99_off[] = {
    {20, &btlgrp_dou_dou_13_99_off_01, &dou_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_dou_tbl[] = {
    {
        /*"マップstg_05_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_05_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_05_5"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x35", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU01エルモス"*/"\x44\x4F\x55\x30\x31\x83\x47\x83\x8B\x83\x82\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_01_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU02バブル"*/"\x44\x4F\x55\x30\x32\x83\x6F\x83\x75\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_02_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU02バブル"*/"\x44\x4F\x55\x30\x32\x83\x6F\x83\x75\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU03キラー大砲"*/"\x44\x4F\x55\x30\x33\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU03キラー"*/"\x44\x4F\x55\x30\x33\x83\x4C\x83\x89\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_03_99_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU03ヘビーボム"*/"\x44\x4F\x55\x30\x33\x83\x77\x83\x72\x81\x5B\x83\x7B\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_03_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU03ヘビーボム"*/"\x44\x4F\x55\x30\x33\x83\x77\x83\x72\x81\x5B\x83\x7B\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_03_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU05エルモス"*/"\x44\x4F\x55\x30\x35\x83\x47\x83\x8B\x83\x82\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_05_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU05パタメット"*/"\x44\x4F\x55\x30\x35\x83\x70\x83\x5E\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_05_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU06パタメット"*/"\x44\x4F\x55\x30\x36\x83\x70\x83\x5E\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_06_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU06パタメット"*/"\x44\x4F\x55\x30\x36\x83\x70\x83\x5E\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_06_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU06パタメット"*/"\x44\x4F\x55\x30\x36\x83\x70\x83\x5E\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_06_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU07エルモス"*/"\x44\x4F\x55\x30\x37\x83\x47\x83\x8B\x83\x82\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_07_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU08ヘビーボム"*/"\x44\x4F\x55\x30\x38\x83\x77\x83\x72\x81\x5B\x83\x7B\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_08_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU09バブル"*/"\x44\x4F\x55\x30\x39\x83\x6F\x83\x75\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_09_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU09エルモス"*/"\x44\x4F\x55\x30\x39\x83\x47\x83\x8B\x83\x82\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_09_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU12キラー大砲"*/"\x44\x4F\x55\x31\x32\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_12_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU12キラー大砲"*/"\x44\x4F\x55\x31\x32\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_12_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU12キラー"*/"\x44\x4F\x55\x31\x32\x83\x4C\x83\x89\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_12_99_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU13キラー大砲"*/"\x44\x4F\x55\x31\x33\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_13_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU13キラー大砲"*/"\x44\x4F\x55\x31\x33\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_13_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU13キラー大砲"*/"\x44\x4F\x55\x31\x33\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_13_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU13キラー大砲"*/"\x44\x4F\x55\x31\x33\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_13_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"DOU13キラー"*/"\x44\x4F\x55\x31\x33\x83\x4C\x83\x89\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_dou_btlno_dou_13_99_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_dou_setup_no_tbl[] = {
    {"BTLNO_DOU_TEST1", 0},
    {"BTLNO_DOU_TEST2", 1},
    {"BTLNO_DOU_TEST3", 2},
    {"BTLNO_DOU_01_01", 3},
    {"BTLNO_DOU_02_01", 4},
    {"BTLNO_DOU_02_02", 5},
    {"BTLNO_DOU_03_01", 6},
    {"BTLNO_DOU_03_99", 7},
    {"BTLNO_DOU_03_02", 8},
    {"BTLNO_DOU_03_03", 9},
    {"BTLNO_DOU_05_01", 10},
    {"BTLNO_DOU_05_02", 11},
    {"BTLNO_DOU_06_01", 12},
    {"BTLNO_DOU_06_02", 13},
    {"BTLNO_DOU_06_03", 14},
    {"BTLNO_DOU_07_01", 15},
    {"BTLNO_DOU_08_01", 16},
    {"BTLNO_DOU_09_01", 17},
    {"BTLNO_DOU_09_02", 18},
    {"BTLNO_DOU_12_01", 19},
    {"BTLNO_DOU_12_02", 20},
    {"BTLNO_DOU_12_99", 21},
    {"BTLNO_DOU_13_01", 22},
    {"BTLNO_DOU_13_02", 23},
    {"BTLNO_DOU_13_03", 24},
    {"BTLNO_DOU_13_04", 25},
    {"BTLNO_DOU_13_99", 26},
    {0},
};

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
    BattleStageObject* obj = BattleSearchObjectPtr(name);

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

EVT_BEGIN(bgset_dou_wave_event)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 200)
    SET_VALUE(LW(2), 200)
    SET_VALUE(LW(3), 100)
    SET_VALUE(LW(4), 300)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 400)
    SET_VALUE(LW(7), 2500)
    LABEL(0)
    CALL(_wave_offset, STRING("nami_00"), LW(0), 800, 30, LW(1), 300, 10)
    CALL(_wave_offset, STRING("nami_01"), LW(2), 600, 20, LW(3), 250, 6)
    CALL(_wave_offset, STRING("nami_02"), LW(4), 1100, 40, LW(5), 360, 7)
    CALL(_wave_offset, STRING("nami_03"), LW(6), 1250, 50, LW(7), 450, 8)
    CALL(_wave_offset, STRING("nami_04"), LW(8), 1100, 50, LW(9), 400, 6)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

