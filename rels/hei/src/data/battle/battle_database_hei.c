#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "hei.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_gold_chorobon;

BattleStageObjectData hei_btlobject_data[59];
BattleStageData hei_btlstage_data[11];
BattleUnitSetup btlparty_hei_hei_01_01_off_1[4];
BattleGroupSetup btlgrp_hei_hei_01_01_off_1;
BattleUnitSetup btlparty_hei_hei_02_02_off_1[2];
BattleGroupSetup btlgrp_hei_hei_02_02_off_1;
BattleUnitSetup btlparty_hei_hei_02_03_off_1[2];
BattleGroupSetup btlgrp_hei_hei_02_03_off_1;
BattleUnitSetup btlparty_hei_hei_02_04_off_1[2];
BattleGroupSetup btlgrp_hei_hei_02_04_off_1;
BattleUnitSetup btlparty_hei_hei_03_01_off_1[2];
BattleGroupSetup btlgrp_hei_hei_03_01_off_1;
BattleUnitSetup btlparty_hei_hei_04_01_off_1[3];
BattleGroupSetup btlgrp_hei_hei_04_01_off_1;
BattleUnitSetup btlparty_hei_hei_04_01_off_2[2];
BattleGroupSetup btlgrp_hei_hei_04_01_off_2;
BattleUnitSetup btlparty_hei_hei_04_02_off_1[2];
BattleGroupSetup btlgrp_hei_hei_04_02_off_1;
BattleUnitSetup btlparty_hei_hei_04_02_off_2[3];
BattleGroupSetup btlgrp_hei_hei_04_02_off_2;
BattleUnitSetup btlparty_hei_hei_05_01_off_1[2];
BattleGroupSetup btlgrp_hei_hei_05_01_off_1;
BattleUnitSetup btlparty_hei_hei_06_02_off_1[2];
BattleGroupSetup btlgrp_hei_hei_06_02_off_1;
BattleUnitSetup btlparty_hei_hei_06_03_off_1[3];
BattleGroupSetup btlgrp_hei_hei_06_03_off_1;
BattleUnitSetup btlparty_hei_hei_06_03_off_2[3];
BattleGroupSetup btlgrp_hei_hei_06_03_off_2;
BattleUnitSetup btlparty_hei_hei_07_01_off_1[4];
BattleGroupSetup btlgrp_hei_hei_07_01_off_1;
BattleUnitSetup btlparty_hei_hei_08_01_off_1[4];
BattleGroupSetup btlgrp_hei_hei_08_01_off_1;
BattleUnitSetup btlparty_hei_hei_09_01_off_1[2];
BattleGroupSetup btlgrp_hei_hei_09_01_off_1;
BattleUnitSetup btlparty_hei_hei_09_01_off_2[3];
BattleGroupSetup btlgrp_hei_hei_09_01_off_2;
BattleUnitSetup btlparty_hei_hei_09_03_off_1[2];
BattleGroupSetup btlgrp_hei_hei_09_03_off_1;
BattleUnitSetup btlparty_hei_hei_09_03_off_2[3];
BattleGroupSetup btlgrp_hei_hei_09_03_off_2;
BattleUnitSetup btlparty_hei_hei_10_01_off_1[1];
BattleGroupSetup btlgrp_hei_hei_10_01_off_1;
BattleUnitSetup btlparty_hei_hei_11_01_off_1[2];
BattleGroupSetup btlgrp_hei_hei_11_01_off_1;
BattleUnitSetup btlparty_hei_hei_11_01_off_2[3];
BattleGroupSetup btlgrp_hei_hei_11_01_off_2;
BattleUnitSetup btlparty_hei_hei_11_03_off_1[2];
BattleGroupSetup btlgrp_hei_hei_11_03_off_1;
BattleUnitSetup btlparty_hei_hei_11_03_off_2[3];
BattleGroupSetup btlgrp_hei_hei_11_03_off_2;
BattleUnitSetup btlparty_hei_hei_12_01_off_1[4];
BattleGroupSetup btlgrp_hei_hei_12_01_off_1;
BattleUnitSetup btlparty_hei_hei_13_02_off_1[2];
BattleGroupSetup btlgrp_hei_hei_13_02_off_1;
BattleUnitSetup btlparty_hei_hei_13_02_on_1[3];
BattleGroupSetup btlgrp_hei_hei_13_02_on_1;
BattleUnitSetup btlparty_hei_hei_13_02_on_2[4];
BattleGroupSetup btlgrp_hei_hei_13_02_on_2;
BattleUnitSetup btlparty_hei_hei_13_02_on_3[5];
BattleGroupSetup btlgrp_hei_hei_13_02_on_3;
BattleUnitSetup btlparty_hei_hei_13_03_off_1[2];
BattleGroupSetup btlgrp_hei_hei_13_03_off_1;
BattleUnitSetup btlparty_hei_hei_13_03_on_1[2];
BattleGroupSetup btlgrp_hei_hei_13_03_on_1;
BattleUnitSetup btlparty_hei_hei_13_03_on_2[3];
BattleGroupSetup btlgrp_hei_hei_13_03_on_2;
BattleUnitSetup btlparty_hei_hei_13_04_off_1[2];
BattleGroupSetup btlgrp_hei_hei_13_04_off_1;
BattleUnitSetup btlparty_hei_hei_13_04_on_1[2];
BattleGroupSetup btlgrp_hei_hei_13_04_on_1;
BattleUnitSetup btlparty_hei_hei_13_04_on_2[4];
BattleGroupSetup btlgrp_hei_hei_13_04_on_2;
NpcBattleSelect btlgrpsel_hei_btlno_hei_test1_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_test2_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_test3_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_test4_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_01_01_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_02_02_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_02_03_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_02_04_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_03_01_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_04_01_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_04_02_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_05_01_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_06_02_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_06_03_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_07_01_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_08_01_off[2];
NpcBattleSelect R_btlgrpsel_hei_btlno_hei_08_02_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_09_01_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_09_03_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_10_01_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_11_01_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_11_03_off[3];
NpcBattleSelect btlgrpsel_hei_btlno_hei_12_01_off[2];
NpcBattleSelect R_btlgrpsel_hei_btlno_hei_12_02_off[2];
NpcBattleSelect btlgrpsel_hei_btlno_hei_13_02_off[5];
NpcBattleSelect btlgrpsel_hei_btlno_hei_13_03_off[4];
NpcBattleSelect btlgrpsel_hei_btlno_hei_13_04_off[4];
NpcSetupBattleInfo btlsetup_hei_tbl[28];
NpcTerritoryInfo btl_hei_setup_no_tbl[28];

BattleStageObjectData hei_btlobject_data[] = {
    {"ki_1", 2, 0, {136.0f, 0.0f, -63.0f}, 0, 60, {0}},
    {"ki_2", 2, 0, {86.0f, 0.0f, -30.0f}, 10, 50, {0}},
    {"ki_3", 2, 0, {40.0f, 0.0f, -50.0f}, 20, 40, {0}},
    {"kusa_1", 2, 1, {-150.0f, 0.0f, -30.0f}, 0, 60, {0}},
    {"kusa_2", 2, 1, {184.0f, 0.0f, 65.0f}, 10, 50, {0}},
    {"kusa_3", 2, 1, {-165.0f, 0.0f, 49.0f}, 20, 40, {0}},
    {"hana_1", 2, 1, {100.0f, 0.0f, 76.0f}, 30, 30, {0}},
    {"hana_2", 2, 1, {-44.0f, 0.0f, 74.0f}, 0, 60, {0}},
    {"saku_1", 2, 1, {-24.0f, 0.0f, -39.0f}, 10, 50, {0}},
    {"saku_2", 2, 1, {172.0f, 0.0f, -33.0f}, 20, 40, {0}},
    {"saku_3", 2, 1, {-99.0f, 0.0f, -70.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ki_1", 2, 0, {131.0f, 0.0f, -32.0f}, 0, 60, {0}},
    {"kusa_1", 2, 1, {27.0f, 0.0f, -40.0f}, 0, 60, {0}},
    {"gareki_1_1", 2, 1, {127.0f, 0.0f, -56.0f}, 10, 50, {0}},
    {"gareki_1_2", 2, 1, {-52.0f, 0.0f, -43.0f}, 20, 40, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ki_1", 2, 0, {-122.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"toride_1", 2, 0, {93.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"kusa_1", 2, 1, {-73.0f, 0.0f, -31.0f}, 0, 60, {0}},
    {"gareki_1_1", 2, 1, {-241.0f, 0.0f, -44.0f}, 10, 50, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ki_1", 2, 0, {131.0f, 0.0f, -32.0f}, 0, 60, {0}},
    {"kusa_1", 2, 1, {27.0f, 0.0f, -40.0f}, 0, 60, {0}},
    {"gareki_2_1", 2, 1, {127.0f, 0.0f, -56.0f}, 10, 50, {0}},
    {"gareki_2_2", 2, 1, {-52.0f, 0.0f, -43.0f}, 20, 40, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ki_1", 2, 0, {-122.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"toride_2", 2, 0, {93.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"kusa_1", 2, 1, {-195.0f, 0.0f, -11.0f}, 0, 60, {0}},
    {"gareki_2_1", 2, 1, {-275.0f, 0.0f, -46.0f}, 10, 50, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ki_1", 2, 0, {131.0f, 0.0f, -32.0f}, 0, 60, {0}},
    {"kusa_1", 2, 1, {27.0f, 0.0f, -40.0f}, 0, 60, {0}},
    {"gareki_3_1", 2, 1, {127.0f, 0.0f, -56.0f}, 10, 50, {0}},
    {"gareki_3_2", 2, 1, {-52.0f, 0.0f, -43.0f}, 20, 40, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ki_1", 2, 0, {-122.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"toride_3", 2, 0, {93.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"kusa_1", 2, 1, {-85.0f, 0.0f, -27.0f}, 0, 60, {0}},
    {"gareki_3_1", 2, 1, {-240.0f, 0.0f, -48.0f}, 10, 50, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"S_dai_01_3", 2, 0, {-127.0f, 0.0f, -53.0f}, 0, 60, {0}},
    {"S_dai_02_3", 2, 0, {127.0f, 0.0f, -53.0f}, 10, 50, {0}},
    {"C_hei_03", 2, 3, {0}, 0, 60, {0}},
    {"S_dai_01_5", 2, 0, {-127.0f, 0.0f, -53.0f}, 0, 60, {0}},
    {"S_dai_02_5", 2, 0, {127.0f, 0.0f, -53.0f}, 10, 50, {0}},
    {"C_hei_05", 2, 3, {0}, 0, 60, {0}},
    {"B_hei_07", 2, 0, {95.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"C_hei_07", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData hei_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_0", // mSpecificStageDataDir
        13, // mNumObjects
        hei_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_4", // mSpecificStageDataDir
        6, // mNumObjects
        &hei_btlobject_data[13], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_4", // mSpecificStageDataDir
        6, // mNumObjects
        &hei_btlobject_data[19], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_4", // mSpecificStageDataDir
        6, // mNumObjects
        &hei_btlobject_data[25], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_4", // mSpecificStageDataDir
        6, // mNumObjects
        &hei_btlobject_data[31], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_4", // mSpecificStageDataDir
        6, // mNumObjects
        &hei_btlobject_data[37], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_4", // mSpecificStageDataDir
        6, // mNumObjects
        &hei_btlobject_data[43], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_1", // mSpecificStageDataDir
        3, // mNumObjects
        &hei_btlobject_data[49], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_1", // mSpecificStageDataDir
        3, // mNumObjects
        &hei_btlobject_data[52], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_1", // mSpecificStageDataDir
        2, // mNumObjects
        &hei_btlobject_data[55], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_2", // mSpecificStageDataDir
        2, // mNumObjects
        &hei_btlobject_data[57], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_hei_hei_01_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_01_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_02_02_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_02_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_02_03_off_1[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_02_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_02_04_off_1[] = {
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_02_04_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_02_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_03_01_off_1[] = {
    {
        &unit_sinnosuke, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinnosuke, // itemDropTable
    },
    {
        &unit_sinnosuke, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinnosuke, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_03_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_03_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_04_01_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_04_01_off_1 = {
    3, // numEnemies
    btlparty_hei_hei_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_04_01_off_2[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_04_01_off_2 = {
    2, // numEnemies
    btlparty_hei_hei_04_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_04_02_off_1[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_04_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_04_02_off_2[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_04_02_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_04_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_05_01_off_1[] = {
    {
        &unit_togedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {80.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togedaruma, // itemDropTable
    },
    {
        &unit_togedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togedaruma, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_05_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_05_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_06_02_off_1[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_06_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_06_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_06_03_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_06_03_off_1 = {
    3, // numEnemies
    btlparty_hei_hei_06_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_06_03_off_2[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_06_03_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_06_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_07_01_off_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
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
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_07_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_07_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_08_01_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_08_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_08_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_09_01_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_09_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_09_01_off_2[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_09_01_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_09_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_09_03_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_09_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_09_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_09_03_off_2[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_09_03_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_09_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_10_01_off_1[] = {
    {
        &unit_gold_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {125.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gold_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_10_01_off_1 = {
    1, // numEnemies
    btlparty_hei_hei_10_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_11_01_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_11_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_11_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_11_01_off_2[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_11_01_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_11_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_11_03_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_11_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_11_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_11_03_off_2[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_11_03_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_11_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_12_01_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_12_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_12_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_02_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_02_on_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_02_on_1 = {
    3, // numEnemies
    btlparty_hei_hei_13_02_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_02_on_2[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_02_on_2 = {
    4, // numEnemies
    btlparty_hei_hei_13_02_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_02_on_3[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_02_on_3 = {
    5, // numEnemies
    btlparty_hei_hei_13_02_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_03_off_1[] = {
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_03_on_1[] = {
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_03_on_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_03_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_03_on_2[] = {
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_03_on_2 = {
    3, // numEnemies
    btlparty_hei_hei_13_03_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_04_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_04_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_04_on_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_04_on_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_04_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_hei_hei_13_04_on_2[] = {
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_hei_hei_13_04_on_2 = {
    4, // numEnemies
    btlparty_hei_hei_13_04_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_test1_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, hei_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_test2_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, &hei_btlstage_data[7]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_test3_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_test4_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, &hei_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_01_01_off[] = {
    {10, &btlgrp_hei_hei_01_01_off_1, hei_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_02_02_off[] = {
    {10, &btlgrp_hei_hei_02_02_off_1, &hei_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_02_03_off[] = {
    {10, &btlgrp_hei_hei_02_03_off_1, &hei_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_02_04_off[] = {
    {10, &btlgrp_hei_hei_02_04_off_1, &hei_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_03_01_off[] = {
    {10, &btlgrp_hei_hei_03_01_off_1, &hei_btlstage_data[7]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_04_01_off[] = {
    {10, &btlgrp_hei_hei_04_01_off_1, &hei_btlstage_data[4]},
    {10, &btlgrp_hei_hei_04_01_off_2, &hei_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_04_02_off[] = {
    {10, &btlgrp_hei_hei_04_02_off_1, &hei_btlstage_data[3]},
    {10, &btlgrp_hei_hei_04_02_off_2, &hei_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_05_01_off[] = {
    {10, &btlgrp_hei_hei_05_01_off_1, &hei_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_06_02_off[] = {
    {10, &btlgrp_hei_hei_06_02_off_1, &hei_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_06_03_off[] = {
    {10, &btlgrp_hei_hei_06_03_off_1, &hei_btlstage_data[6]},
    {10, &btlgrp_hei_hei_06_03_off_2, &hei_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_07_01_off[] = {
    {10, &btlgrp_hei_hei_07_01_off_1, &hei_btlstage_data[9]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_08_01_off[] = {
    {10, &btlgrp_hei_hei_08_01_off_1, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect R_btlgrpsel_hei_btlno_hei_08_02_off[] = {
    {10, &btlgrp_hei_hei_08_01_off_1, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_09_01_off[] = {
    {10, &btlgrp_hei_hei_09_01_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_09_01_off_2, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_09_03_off[] = {
    {10, &btlgrp_hei_hei_09_03_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_09_03_off_2, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_10_01_off[] = {
    {10, &btlgrp_hei_hei_10_01_off_1, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_11_01_off[] = {
    {10, &btlgrp_hei_hei_11_01_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_11_01_off_2, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_11_03_off[] = {
    {10, &btlgrp_hei_hei_11_03_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_11_03_off_2, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_12_01_off[] = {
    {10, &btlgrp_hei_hei_12_01_off_1, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect R_btlgrpsel_hei_btlno_hei_12_02_off[] = {
    {10, &btlgrp_hei_hei_12_01_off_1, &hei_btlstage_data[10]},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_13_02_off[] = {
    {10, &btlgrp_hei_hei_13_02_on_1, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_02_on_2, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_02_on_3, hei_btlstage_data},
    {50, &btlgrp_hei_hei_13_02_off_1, hei_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_13_03_off[] = {
    {50, &btlgrp_hei_hei_13_03_on_1, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_03_on_2, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_03_off_1, hei_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_hei_btlno_hei_13_04_off[] = {
    {50, &btlgrp_hei_hei_13_04_on_1, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_04_on_2, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_04_off_1, hei_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_hei_tbl[] = {
    {
        /*"マップstg_01_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_01_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_01_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_01_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test4_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei01クリボー"*/"\x68\x65\x69\x30\x31\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_01_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei02パタクリ２"*/"\x68\x65\x69\x30\x32\x83\x70\x83\x5E\x83\x4E\x83\x8A\x82\x51", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei02ノコノコ"*/"\x68\x65\x69\x30\x32\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_02_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei02トゲクリ"*/"\x68\x65\x69\x30\x32\x83\x67\x83\x51\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_02_04_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei03シンノスケ"*/"\x68\x65\x69\x30\x33\x83\x56\x83\x93\x83\x6D\x83\x58\x83\x50", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_03_01_off, // select
        0x2000000F, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        /*"hei04パタクリ"*/"\x68\x65\x69\x30\x34\x83\x70\x83\x5E\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_04_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei04ノコノコ"*/"\x68\x65\x69\x30\x34\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_04_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei05トゲダルマー"*/"\x68\x65\x69\x30\x35\x83\x67\x83\x51\x83\x5F\x83\x8B\x83\x7D\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_05_01_off, // select
        0x2000000F, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        /*"hei06ノコノコ"*/"\x68\x65\x69\x30\x36\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_06_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei06パタクリ２"*/"\x68\x65\x69\x30\x36\x83\x70\x83\x5E\x83\x4E\x83\x8A\x82\x51", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_06_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei07モノクロシンエモン"*/"\x68\x65\x69\x30\x37\x83\x82\x83\x6D\x83\x4E\x83\x8D\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_07_01_off, // select
        0x2000001F, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        /*"hei08チョロボン"*/"\x68\x65\x69\x30\x38\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_08_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei08チョロボン２"*/"\x68\x65\x69\x30\x38\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93\x82\x51", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        R_btlgrpsel_hei_btlno_hei_08_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei09チョロボン1"*/"\x68\x65\x69\x30\x39\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_09_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei09チョロボン3"*/"\x68\x65\x69\x30\x39\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_09_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei10ゴールドチョロボン"*/"\x68\x65\x69\x31\x30\x83\x53\x81\x5B\x83\x8B\x83\x68\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        "hei_10", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_10_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        /*"hei11チョロボン1"*/"\x68\x65\x69\x31\x31\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_11_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei11チョロボン3"*/"\x68\x65\x69\x31\x31\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_11_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei12チョロボン"*/"\x68\x65\x69\x31\x32\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_12_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei12チョロボン２"*/"\x68\x65\x69\x31\x32\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93\x82\x51", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        R_btlgrpsel_hei_btlno_hei_12_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei13クリボー2"*/"\x68\x65\x69\x31\x33\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_13_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei13トゲクリ"*/"\x68\x65\x69\x31\x33\x83\x67\x83\x51\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_13_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"hei13パタクリ"*/"\x68\x65\x69\x31\x33\x83\x70\x83\x5E\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_13_04_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_hei_setup_no_tbl[] = {
    {"BTLNO_HEI_TEST1", 0},
    {"BTLNO_HEI_TEST2", 1},
    {"BTLNO_HEI_TEST3", 2},
    {"BTLNO_HEI_TEST4", 3},
    {"BTLNO_HEI_01_01", 4},
    {"BTLNO_HEI_02_02", 5},
    {"BTLNO_HEI_02_03", 6},
    {"BTLNO_HEI_02_04", 7},
    {"BTLNO_HEI_03_01", 8},
    {"BTLNO_HEI_04_01", 9},
    {"BTLNO_HEI_04_02", 10},
    {"BTLNO_HEI_05_01", 11},
    {"BTLNO_HEI_06_02", 12},
    {"BTLNO_HEI_06_03", 13},
    {"BTLNO_HEI_07_01", 14},
    {"BTLNO_HEI_08_01", 15},
    {"BTLNO_HEI_08_02", 16},
    {"BTLNO_HEI_09_01", 17},
    {"BTLNO_HEI_09_03", 18},
    {"BTLNO_HEI_10_01", 19},
    {"BTLNO_HEI_11_01", 20},
    {"BTLNO_HEI_11_03", 21},
    {"BTLNO_HEI_12_01", 22},
    {"BTLNO_HEI_12_02", 23},
    {"BTLNO_HEI_13_02", 24},
    {"BTLNO_HEI_13_03", 25},
    {"BTLNO_HEI_13_04", 26},
    {0},
};

