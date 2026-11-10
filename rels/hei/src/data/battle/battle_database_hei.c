// Petal Meadows (hei) battle setups: enemy groups, battle stages and the field-battle tables for the area's maps.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "hei.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_gold_chorobon;

static BattleStageObjectData hei_btlobject_data[59];
static BattleStageData hei_btlstage_data[11];
static BattleUnitSetup btlparty_hei_hei_01_01_off_1[4];
static BattleGroupSetup btlgrp_hei_hei_01_01_off_1;
static BattleUnitSetup btlparty_hei_hei_02_02_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_02_02_off_1;
static BattleUnitSetup btlparty_hei_hei_02_03_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_02_03_off_1;
static BattleUnitSetup btlparty_hei_hei_02_04_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_02_04_off_1;
static BattleUnitSetup btlparty_hei_hei_03_01_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_03_01_off_1;
static BattleUnitSetup btlparty_hei_hei_04_01_off_1[3];
static BattleGroupSetup btlgrp_hei_hei_04_01_off_1;
static BattleUnitSetup btlparty_hei_hei_04_01_off_2[2];
static BattleGroupSetup btlgrp_hei_hei_04_01_off_2;
static BattleUnitSetup btlparty_hei_hei_04_02_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_04_02_off_1;
static BattleUnitSetup btlparty_hei_hei_04_02_off_2[3];
static BattleGroupSetup btlgrp_hei_hei_04_02_off_2;
static BattleUnitSetup btlparty_hei_hei_05_01_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_05_01_off_1;
static BattleUnitSetup btlparty_hei_hei_06_02_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_06_02_off_1;
static BattleUnitSetup btlparty_hei_hei_06_03_off_1[3];
static BattleGroupSetup btlgrp_hei_hei_06_03_off_1;
static BattleUnitSetup btlparty_hei_hei_06_03_off_2[3];
static BattleGroupSetup btlgrp_hei_hei_06_03_off_2;
static BattleUnitSetup btlparty_hei_hei_07_01_off_1[4];
static BattleGroupSetup btlgrp_hei_hei_07_01_off_1;
static BattleUnitSetup btlparty_hei_hei_08_01_off_1[4];
static BattleGroupSetup btlgrp_hei_hei_08_01_off_1;
static BattleUnitSetup btlparty_hei_hei_09_01_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_09_01_off_1;
static BattleUnitSetup btlparty_hei_hei_09_01_off_2[3];
static BattleGroupSetup btlgrp_hei_hei_09_01_off_2;
static BattleUnitSetup btlparty_hei_hei_09_03_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_09_03_off_1;
static BattleUnitSetup btlparty_hei_hei_09_03_off_2[3];
static BattleGroupSetup btlgrp_hei_hei_09_03_off_2;
static BattleUnitSetup btlparty_hei_hei_10_01_off_1[1];
static BattleGroupSetup btlgrp_hei_hei_10_01_off_1;
static BattleUnitSetup btlparty_hei_hei_11_01_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_11_01_off_1;
static BattleUnitSetup btlparty_hei_hei_11_01_off_2[3];
static BattleGroupSetup btlgrp_hei_hei_11_01_off_2;
static BattleUnitSetup btlparty_hei_hei_11_03_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_11_03_off_1;
static BattleUnitSetup btlparty_hei_hei_11_03_off_2[3];
static BattleGroupSetup btlgrp_hei_hei_11_03_off_2;
static BattleUnitSetup btlparty_hei_hei_12_01_off_1[4];
static BattleGroupSetup btlgrp_hei_hei_12_01_off_1;
static BattleUnitSetup btlparty_hei_hei_13_02_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_13_02_off_1;
static BattleUnitSetup btlparty_hei_hei_13_02_on_1[3];
static BattleGroupSetup btlgrp_hei_hei_13_02_on_1;
static BattleUnitSetup btlparty_hei_hei_13_02_on_2[4];
static BattleGroupSetup btlgrp_hei_hei_13_02_on_2;
static BattleUnitSetup btlparty_hei_hei_13_02_on_3[5];
static BattleGroupSetup btlgrp_hei_hei_13_02_on_3;
static BattleUnitSetup btlparty_hei_hei_13_03_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_13_03_off_1;
static BattleUnitSetup btlparty_hei_hei_13_03_on_1[2];
static BattleGroupSetup btlgrp_hei_hei_13_03_on_1;
static BattleUnitSetup btlparty_hei_hei_13_03_on_2[3];
static BattleGroupSetup btlgrp_hei_hei_13_03_on_2;
static BattleUnitSetup btlparty_hei_hei_13_04_off_1[2];
static BattleGroupSetup btlgrp_hei_hei_13_04_off_1;
static BattleUnitSetup btlparty_hei_hei_13_04_on_1[2];
static BattleGroupSetup btlgrp_hei_hei_13_04_on_1;
static BattleUnitSetup btlparty_hei_hei_13_04_on_2[4];
static BattleGroupSetup btlgrp_hei_hei_13_04_on_2;
static NpcBattleSelect btlgrpsel_hei_btlno_hei_test1_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_test2_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_test3_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_test4_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_01_01_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_02_02_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_02_03_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_02_04_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_03_01_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_04_01_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_04_02_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_05_01_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_06_02_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_06_03_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_07_01_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_08_01_off[2];

#if VERSION >= VERSION_JP
NpcBattleSelect R_btlgrpsel_hei_btlno_hei_08_02_off[2];
#endif

static NpcBattleSelect btlgrpsel_hei_btlno_hei_09_01_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_09_03_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_10_01_off[2];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_11_01_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_11_03_off[3];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_12_01_off[2];

#if VERSION >= VERSION_JP
NpcBattleSelect R_btlgrpsel_hei_btlno_hei_12_02_off[2];
#endif

static NpcBattleSelect btlgrpsel_hei_btlno_hei_13_02_off[5];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_13_03_off[4];
static NpcBattleSelect btlgrpsel_hei_btlno_hei_13_04_off[4];

#if VERSION <= VERSION_DEMO_JP
NpcSetupBattleInfo btlsetup_hei_tbl[26];
NpcTerritoryInfo btl_hei_setup_no_tbl[26];
#else
NpcSetupBattleInfo btlsetup_hei_tbl[28];
NpcTerritoryInfo btl_hei_setup_no_tbl[28];
#endif

/**
 * Scenery objects of the Petal Meadows battle stages (trees, grass, flowers, fences, rubble, the Shhwonk
 * Fortress statues and the back and front wall pieces); each stage in hei_btlstage_data uses a slice of this
 * list.
 */
static BattleStageObjectData hei_btlobject_data[] = {
    {"ki_1", 2, STAGE_LAYER_A1, {136.0f, 0.0f, -63.0f}, 0, 60, {0}},
    {"ki_2", 2, STAGE_LAYER_A1, {86.0f, 0.0f, -30.0f}, 10, 50, {0}},
    {"ki_3", 2, STAGE_LAYER_A1, {40.0f, 0.0f, -50.0f}, 20, 40, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {-150.0f, 0.0f, -30.0f}, 0, 60, {0}},
    {"kusa_2", 2, STAGE_LAYER_A2, {184.0f, 0.0f, 65.0f}, 10, 50, {0}},
    {"kusa_3", 2, STAGE_LAYER_A2, {-165.0f, 0.0f, 49.0f}, 20, 40, {0}},
    {"hana_1", 2, STAGE_LAYER_A2, {100.0f, 0.0f, 76.0f}, 30, 30, {0}},
    {"hana_2", 2, STAGE_LAYER_A2, {-44.0f, 0.0f, 74.0f}, 0, 60, {0}},
#if VERSION <= VERSION_DEMO_JP
    {"saku_1", 2, STAGE_LAYER_A2, {-11.0f, 0.0f, -24.0f}, 10, 50, {0}},
#else
    {"saku_1", 2, STAGE_LAYER_A2, {-24.0f, 0.0f, -39.0f}, 10, 50, {0}},
#endif
    {"saku_2", 2, STAGE_LAYER_A2, {172.0f, 0.0f, -33.0f}, 20, 40, {0}},
    {"saku_3", 2, STAGE_LAYER_A2, {-99.0f, 0.0f, -70.0f}, 30, 30, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ki_1", 2, STAGE_LAYER_A1, {131.0f, 0.0f, -32.0f}, 0, 60, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {27.0f, 0.0f, -40.0f}, 0, 60, {0}},
    {"gareki_1_1", 2, STAGE_LAYER_A2, {127.0f, 0.0f, -56.0f}, 10, 50, {0}},
    {"gareki_1_2", 2, STAGE_LAYER_A2, {-52.0f, 0.0f, -43.0f}, 20, 40, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ki_1", 2, STAGE_LAYER_A1, {-122.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"toride_1", 2, STAGE_LAYER_A1, {93.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {-73.0f, 0.0f, -31.0f}, 0, 60, {0}},
    {"gareki_1_1", 2, STAGE_LAYER_A2, {-241.0f, 0.0f, -44.0f}, 10, 50, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ki_1", 2, STAGE_LAYER_A1, {131.0f, 0.0f, -32.0f}, 0, 60, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {27.0f, 0.0f, -40.0f}, 0, 60, {0}},
    {"gareki_2_1", 2, STAGE_LAYER_A2, {127.0f, 0.0f, -56.0f}, 10, 50, {0}},
    {"gareki_2_2", 2, STAGE_LAYER_A2, {-52.0f, 0.0f, -43.0f}, 20, 40, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ki_1", 2, STAGE_LAYER_A1, {-122.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"toride_2", 2, STAGE_LAYER_A1, {93.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {-195.0f, 0.0f, -11.0f}, 0, 60, {0}},
    {"gareki_2_1", 2, STAGE_LAYER_A2, {-275.0f, 0.0f, -46.0f}, 10, 50, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ki_1", 2, STAGE_LAYER_A1, {131.0f, 0.0f, -32.0f}, 0, 60, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {27.0f, 0.0f, -40.0f}, 0, 60, {0}},
    {"gareki_3_1", 2, STAGE_LAYER_A2, {127.0f, 0.0f, -56.0f}, 10, 50, {0}},
    {"gareki_3_2", 2, STAGE_LAYER_A2, {-52.0f, 0.0f, -43.0f}, 20, 40, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ki_1", 2, STAGE_LAYER_A1, {-122.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"toride_3", 2, STAGE_LAYER_A1, {93.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"kusa_1", 2, STAGE_LAYER_A2, {-85.0f, 0.0f, -27.0f}, 0, 60, {0}},
    {"gareki_3_1", 2, STAGE_LAYER_A2, {-240.0f, 0.0f, -48.0f}, 10, 50, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"S_dai_01_3", 2, STAGE_LAYER_A1, {-127.0f, 0.0f, -53.0f}, 0, 60, {0}},
    {"S_dai_02_3", 2, STAGE_LAYER_A1, {127.0f, 0.0f, -53.0f}, 10, 50, {0}},
    {"C_hei_03", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"S_dai_01_5", 2, STAGE_LAYER_A1, {-127.0f, 0.0f, -53.0f}, 0, 60, {0}},
    {"S_dai_02_5", 2, STAGE_LAYER_A1, {127.0f, 0.0f, -53.0f}, 10, 50, {0}},
    {"C_hei_05", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B_hei_07", 2, STAGE_LAYER_A1, {95.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"C_hei_07", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
};

/**
 * The 11 battle stages of Petal Meadows: each pairs a stage model (stg_01_0, stg_01_4, stg_01_1, stg_01_2)
 * with a slice of the scenery objects above.
 */
static BattleStageData hei_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_01_0", // specificStageDataDir
        13, // numObjects
        hei_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_4", // specificStageDataDir
        6, // numObjects
        &hei_btlobject_data[13], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_4", // specificStageDataDir
        6, // numObjects
        &hei_btlobject_data[19], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_4", // specificStageDataDir
        6, // numObjects
        &hei_btlobject_data[25], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_4", // specificStageDataDir
        6, // numObjects
        &hei_btlobject_data[31], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_4", // specificStageDataDir
        6, // numObjects
        &hei_btlobject_data[37], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_4", // specificStageDataDir
        6, // numObjects
        &hei_btlobject_data[43], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_1", // specificStageDataDir
        3, // numObjects
        &hei_btlobject_data[49], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_1", // specificStageDataDir
        3, // numObjects
        &hei_btlobject_data[52], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_1", // specificStageDataDir
        2, // numObjects
        &hei_btlobject_data[55], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_01_2", // specificStageDataDir
        2, // numObjects
        &hei_btlobject_data[57], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

/** Enemy lineup of formation 1 of BTLNO_HEI_01_01, in Petal Meadows: Goomba x2, Spiky Goomba, Paragoomba. */
static BattleUnitSetup btlparty_hei_hei_01_01_off_1[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_01_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_01_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_02_02, in Petal Meadows: Paragoomba, Koopa Troopa. */
static BattleUnitSetup btlparty_hei_hei_02_02_off_1[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_02_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_02_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_02_03, in Petal Meadows: Koopa Troopa, Goomba. */
static BattleUnitSetup btlparty_hei_hei_02_03_off_1[] = {
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_02_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_02_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_02_04, in Petal Meadows: Spiky Goomba, Koopa Troopa. */
static BattleUnitSetup btlparty_hei_hei_02_04_off_1[] = {
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_02_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_02_04_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_02_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_03_01, in Petal Meadows: Bald Cleft x2. */
static BattleUnitSetup btlparty_hei_hei_03_01_off_1[] = {
    {
        &unit_sinnosuke, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinnosuke, // itemDropTable
    },
    {
        &unit_sinnosuke, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinnosuke, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_03_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 0 held, 0 random, 100 none.
 */
static BattleGroupSetup btlgrp_hei_hei_03_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_03_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_04_01, in Petal Meadows: Paragoomba x3. */
static BattleUnitSetup btlparty_hei_hei_04_01_off_1[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_04_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_04_01_off_1 = {
    3, // numEnemies
    btlparty_hei_hei_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_04_01, in Petal Meadows: Paragoomba, Koopa Troopa. */
static BattleUnitSetup btlparty_hei_hei_04_01_off_2[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_04_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_04_01_off_2 = {
    2, // numEnemies
    btlparty_hei_hei_04_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_04_02, in Petal Meadows: Koopa Troopa x2. */
static BattleUnitSetup btlparty_hei_hei_04_02_off_1[] = {
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_04_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_04_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_04_02, in Petal Meadows: Koopa Troopa x3. */
static BattleUnitSetup btlparty_hei_hei_04_02_off_2[] = {
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_04_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_04_02_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_04_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_05_01, in Petal Meadows: Bristle x2. */
static BattleUnitSetup btlparty_hei_hei_05_01_off_1[] = {
    {
        &unit_togedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {80.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togedaruma, // itemDropTable
    },
    {
        &unit_togedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togedaruma, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_05_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 0 held, 0 random, 100 none.
 */
static BattleGroupSetup btlgrp_hei_hei_05_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_05_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_06_02, in Petal Meadows: Koopa Troopa, Spiky Goomba. */
static BattleUnitSetup btlparty_hei_hei_06_02_off_1[] = {
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_06_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_06_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_06_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_06_03, in Petal Meadows: Paragoomba, Koopa Troopa x2. */
static BattleUnitSetup btlparty_hei_hei_06_03_off_1[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_06_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_06_03_off_1 = {
    3, // numEnemies
    btlparty_hei_hei_06_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_06_03, in Petal Meadows: Paragoomba x2, Koopa Troopa. */
static BattleUnitSetup btlparty_hei_hei_06_03_off_2[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_nokonoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_06_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_06_03_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_06_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_07_01, in Petal Meadows: Cleft x4. */
static BattleUnitSetup btlparty_hei_hei_07_01_off_1[] = {
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_07_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 0 held, 0 random, 100 none.
 */
static BattleGroupSetup btlgrp_hei_hei_07_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_07_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_08_01 and BTLNO_HEI_08_02, in Petal Meadows: Fuzzy x4. */
static BattleUnitSetup btlparty_hei_hei_08_01_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_08_01 and BTLNO_HEI_08_02: the lineup above with the default heart and flower
 * drops. Item drop weights: 0 held, 0 random, 100 none.
 */
static BattleGroupSetup btlgrp_hei_hei_08_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_08_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_09_01, in Petal Meadows: Fuzzy x2. */
static BattleUnitSetup btlparty_hei_hei_09_01_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_09_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_09_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_09_01, in Petal Meadows: Fuzzy x3. */
static BattleUnitSetup btlparty_hei_hei_09_01_off_2[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_09_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_09_01_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_09_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_09_03, in Petal Meadows: Fuzzy x2. */
static BattleUnitSetup btlparty_hei_hei_09_03_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_09_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_09_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_09_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_09_03, in Petal Meadows: Fuzzy x3. */
static BattleUnitSetup btlparty_hei_hei_09_03_off_2[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_09_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_09_03_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_09_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

// The initial group contains only Gold Fuzzy; its damage event summons the horde later.
static BattleUnitSetup btlparty_hei_hei_10_01_off_1[] = {
    {
        &unit_gold_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {125.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gold_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_10_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 0 held, 0 random, 100 none.
 */
static BattleGroupSetup btlgrp_hei_hei_10_01_off_1 = {
    1, // numEnemies
    btlparty_hei_hei_10_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_11_01, in Petal Meadows: Fuzzy x2. */
static BattleUnitSetup btlparty_hei_hei_11_01_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_11_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_11_01_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_11_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_11_01, in Petal Meadows: Fuzzy x3. */
static BattleUnitSetup btlparty_hei_hei_11_01_off_2[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_11_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_11_01_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_11_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_11_03, in Petal Meadows: Fuzzy x2. */
static BattleUnitSetup btlparty_hei_hei_11_03_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_11_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_11_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_11_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_11_03, in Petal Meadows: Fuzzy x3. */
static BattleUnitSetup btlparty_hei_hei_11_03_off_2[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_11_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_11_03_off_2 = {
    3, // numEnemies
    btlparty_hei_hei_11_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_12_01 and BTLNO_HEI_12_02, in Petal Meadows: Fuzzy x4. */
static BattleUnitSetup btlparty_hei_hei_12_01_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_12_01 and BTLNO_HEI_12_02: the lineup above with the default heart and flower
 * drops. Item drop weights: 0 held, 0 random, 100 none.
 */
static BattleGroupSetup btlgrp_hei_hei_12_01_off_1 = {
    4, // numEnemies
    btlparty_hei_hei_12_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_13_02, in Petal Meadows: Goomba, Spiky Goomba. */
static BattleUnitSetup btlparty_hei_hei_13_02_off_1[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_13_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_02_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_13_02, in Petal Meadows: Goomba x3. */
static BattleUnitSetup btlparty_hei_hei_13_02_on_1[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_13_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_02_on_1 = {
    3, // numEnemies
    btlparty_hei_hei_13_02_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_13_02, in Petal Meadows: Goomba x4. */
static BattleUnitSetup btlparty_hei_hei_13_02_on_2[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_13_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_02_on_2 = {
    4, // numEnemies
    btlparty_hei_hei_13_02_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 3 of BTLNO_HEI_13_02, in Petal Meadows: Goomba x5. */
static BattleUnitSetup btlparty_hei_hei_13_02_on_3[] = {
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
    {
        &unit_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_HEI_13_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_02_on_3 = {
    5, // numEnemies
    btlparty_hei_hei_13_02_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_13_03, in Petal Meadows: Spiky Goomba x2. */
static BattleUnitSetup btlparty_hei_hei_13_03_off_1[] = {
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_13_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_03_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_13_03, in Petal Meadows: Spiky Goomba x2. */
static BattleUnitSetup btlparty_hei_hei_13_03_on_1[] = {
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_13_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_03_on_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_03_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_13_03, in Petal Meadows: Spiky Goomba x3. */
static BattleUnitSetup btlparty_hei_hei_13_03_on_2[] = {
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_13_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_03_on_2 = {
    3, // numEnemies
    btlparty_hei_hei_13_03_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_13_04, in Petal Meadows: Paragoomba x2. */
static BattleUnitSetup btlparty_hei_hei_13_04_off_1[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_13_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_04_off_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_HEI_13_04, in Petal Meadows: Paragoomba x2. */
static BattleUnitSetup btlparty_hei_hei_13_04_on_1[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_HEI_13_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_04_on_1 = {
    2, // numEnemies
    btlparty_hei_hei_13_04_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_HEI_13_04, in Petal Meadows: Paragoomba x4. */
static BattleUnitSetup btlparty_hei_hei_13_04_on_2[] = {
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_HEI_13_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
static BattleGroupSetup btlgrp_hei_hei_13_04_on_2 = {
    4, // numEnemies
    btlparty_hei_hei_13_04_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

static NpcBattleSelect btlgrpsel_hei_btlno_hei_test1_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, hei_btlstage_data},
    {0},
};

static NpcBattleSelect btlgrpsel_hei_btlno_hei_test2_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, &hei_btlstage_data[7]},
    {0},
};

static NpcBattleSelect btlgrpsel_hei_btlno_hei_test3_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, &hei_btlstage_data[10]},
    {0},
};

static NpcBattleSelect btlgrpsel_hei_btlno_hei_test4_off[] = {
    {50, &btlgrp_hei_hei_01_01_off_1, &hei_btlstage_data[1]},
    {0},
};

/** BTLNO_HEI_01_01 always fights Goomba x2, Spiky Goomba, Paragoomba on stage stg_01_0. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_01_01_off[] = {
    {10, &btlgrp_hei_hei_01_01_off_1, hei_btlstage_data},
    {0},
};

/** BTLNO_HEI_02_02 always fights Paragoomba, Koopa Troopa on stage stg_01_4. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_02_02_off[] = {
    {10, &btlgrp_hei_hei_02_02_off_1, &hei_btlstage_data[1]},
    {0},
};

/** BTLNO_HEI_02_03 always fights Koopa Troopa, Goomba on stage stg_01_4. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_02_03_off[] = {
    {10, &btlgrp_hei_hei_02_03_off_1, &hei_btlstage_data[1]},
    {0},
};

/** BTLNO_HEI_02_04 always fights Spiky Goomba, Koopa Troopa on stage stg_01_4. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_02_04_off[] = {
    {10, &btlgrp_hei_hei_02_04_off_1, &hei_btlstage_data[2]},
    {0},
};

/** BTLNO_HEI_03_01 always fights Bald Cleft x2 on stage stg_01_1. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_03_01_off[] = {
    {10, &btlgrp_hei_hei_03_01_off_1, &hei_btlstage_data[7]},
    {0},
};

/**
 * BTLNO_HEI_04_01 picks one of its formations by weight (10:10) on stage stg_01_4: Paragoomba x3;
 * Paragoomba, Koopa Troopa.
 */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_04_01_off[] = {
    {10, &btlgrp_hei_hei_04_01_off_1, &hei_btlstage_data[4]},
    {10, &btlgrp_hei_hei_04_01_off_2, &hei_btlstage_data[4]},
    {0},
};

/**
 * BTLNO_HEI_04_02 picks one of its formations by weight (10:10) on stage stg_01_4: Koopa Troopa x2; Koopa
 * Troopa x3.
 */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_04_02_off[] = {
    {10, &btlgrp_hei_hei_04_02_off_1, &hei_btlstage_data[3]},
    {10, &btlgrp_hei_hei_04_02_off_2, &hei_btlstage_data[3]},
    {0},
};

/** BTLNO_HEI_05_01 always fights Bristle x2 on stage stg_01_1. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_05_01_off[] = {
    {10, &btlgrp_hei_hei_05_01_off_1, &hei_btlstage_data[8]},
    {0},
};

/** BTLNO_HEI_06_02 always fights Koopa Troopa, Spiky Goomba on stage stg_01_4. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_06_02_off[] = {
    {10, &btlgrp_hei_hei_06_02_off_1, &hei_btlstage_data[5]},
    {0},
};

/**
 * BTLNO_HEI_06_03 picks one of its formations by weight (10:10) on stage stg_01_4: Paragoomba, Koopa Troopa
 * x2; Paragoomba x2, Koopa Troopa.
 */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_06_03_off[] = {
    {10, &btlgrp_hei_hei_06_03_off_1, &hei_btlstage_data[6]},
    {10, &btlgrp_hei_hei_06_03_off_2, &hei_btlstage_data[6]},
    {0},
};

/** BTLNO_HEI_07_01 always fights Cleft x4 on stage stg_01_1. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_07_01_off[] = {
    {10, &btlgrp_hei_hei_07_01_off_1, &hei_btlstage_data[9]},
    {0},
};

/** BTLNO_HEI_08_01 always fights Fuzzy x4 on stage stg_01_2. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_08_01_off[] = {
    {10, &btlgrp_hei_hei_08_01_off_1, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_08_02 always fights Fuzzy x4 on stage stg_01_2. */
#if VERSION >= VERSION_JP
NpcBattleSelect R_btlgrpsel_hei_btlno_hei_08_02_off[] = {
    {10, &btlgrp_hei_hei_08_01_off_1, &hei_btlstage_data[10]},
    {0},
};
#endif

/** BTLNO_HEI_09_01 picks one of its formations by weight (10:10) on stage stg_01_2: Fuzzy x2; Fuzzy x3. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_09_01_off[] = {
    {10, &btlgrp_hei_hei_09_01_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_09_01_off_2, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_09_03 picks one of its formations by weight (10:10) on stage stg_01_2: Fuzzy x2; Fuzzy x3. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_09_03_off[] = {
    {10, &btlgrp_hei_hei_09_03_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_09_03_off_2, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_10_01 always fights Gold Fuzzy on stage stg_01_2. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_10_01_off[] = {
    {10, &btlgrp_hei_hei_10_01_off_1, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_11_01 picks one of its formations by weight (10:10) on stage stg_01_2: Fuzzy x2; Fuzzy x3. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_11_01_off[] = {
    {10, &btlgrp_hei_hei_11_01_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_11_01_off_2, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_11_03 picks one of its formations by weight (10:10) on stage stg_01_2: Fuzzy x2; Fuzzy x3. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_11_03_off[] = {
    {10, &btlgrp_hei_hei_11_03_off_1, &hei_btlstage_data[10]},
    {10, &btlgrp_hei_hei_11_03_off_2, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_12_01 always fights Fuzzy x4 on stage stg_01_2. */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_12_01_off[] = {
    {10, &btlgrp_hei_hei_12_01_off_1, &hei_btlstage_data[10]},
    {0},
};

/** BTLNO_HEI_12_02 always fights Fuzzy x4 on stage stg_01_2. */
#if VERSION >= VERSION_JP
NpcBattleSelect R_btlgrpsel_hei_btlno_hei_12_02_off[] = {
    {10, &btlgrp_hei_hei_12_01_off_1, &hei_btlstage_data[10]},
    {0},
};
#endif

// Relative selection weights: three Goomba-only groups at 10 each, then the mixed group at 50.
static NpcBattleSelect btlgrpsel_hei_btlno_hei_13_02_off[] = {
    {10, &btlgrp_hei_hei_13_02_on_1, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_02_on_2, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_02_on_3, hei_btlstage_data},
    {50, &btlgrp_hei_hei_13_02_off_1, hei_btlstage_data},
    {0},
};

/**
 * BTLNO_HEI_13_03 picks one of its formations by weight (50:10:10) on stage stg_01_0: Spiky Goomba x2; Spiky
 * Goomba x3; Spiky Goomba x2.
 */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_13_03_off[] = {
    {50, &btlgrp_hei_hei_13_03_on_1, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_03_on_2, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_03_off_1, hei_btlstage_data},
    {0},
};

/**
 * BTLNO_HEI_13_04 picks one of its formations by weight (50:10:10) on stage stg_01_0: Paragoomba x2;
 * Paragoomba x4; Paragoomba x2.
 */
static NpcBattleSelect btlgrpsel_hei_btlno_hei_13_04_off[] = {
    {50, &btlgrp_hei_hei_13_04_on_1, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_04_on_2, hei_btlstage_data},
    {10, &btlgrp_hei_hei_13_04_off_1, hei_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_hei_tbl[] = {
    {
        "マップstg_01_0", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_01_1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_01_2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test3_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_01_4", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_test4_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei01クリボー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_01_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei02パタクリ２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_02_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei02ノコノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_02_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei02トゲクリ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_02_04_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei03シンノスケ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_03_01_off, // select
        (NPC_BATTLE_NO_COIN_DROP | NPC_BATTLE_NO_HEART_DROP | NPC_BATTLE_NO_FLOWER_DROP | NPC_BATTLE_NO_ITEM_DROP | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "hei04パタクリ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_04_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei04ノコノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_04_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei05トゲダルマー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_05_01_off, // select
        (NPC_BATTLE_NO_COIN_DROP | NPC_BATTLE_NO_HEART_DROP | NPC_BATTLE_NO_FLOWER_DROP | NPC_BATTLE_NO_ITEM_DROP | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "hei06ノコノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_06_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei06パタクリ２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_06_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei07モノクロシンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_07_01_off, // select
        (NPC_BATTLE_NO_COIN_DROP | NPC_BATTLE_NO_HEART_DROP | NPC_BATTLE_NO_FLOWER_DROP | NPC_BATTLE_NO_ITEM_DROP | NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "hei08チョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_08_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
#if VERSION >= VERSION_JP
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei08チョロボン２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        R_btlgrpsel_hei_btlno_hei_08_02_off, // select
        0, 0, // battleFlags..randomMode
#endif
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei09チョロボン1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_09_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei09チョロボン3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_09_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei10ゴールドチョロボン", // name
        "hei_10", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_10_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "hei11チョロボン1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_11_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei11チョロボン3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_11_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei12チョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_12_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
#if VERSION >= VERSION_JP
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei12チョロボン２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        R_btlgrpsel_hei_btlno_hei_12_02_off, // select
        0, 0, // battleFlags..randomMode
#endif
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei13クリボー2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_13_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei13トゲクリ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_13_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "hei13パタクリ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_hei_btlno_hei_13_04_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // randomRanges
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
#if VERSION <= VERSION_DEMO_JP
    {"BTLNO_HEI_09_01", 16},
    {"BTLNO_HEI_09_03", 17},
    {"BTLNO_HEI_10_01", 18},
    {"BTLNO_HEI_11_01", 19},
    {"BTLNO_HEI_11_03", 20},
    {"BTLNO_HEI_12_01", 21},
    {"BTLNO_HEI_13_02", 22},
    {"BTLNO_HEI_13_03", 23},
    {"BTLNO_HEI_13_04", 24},
#else
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
#endif
    {0},
};
