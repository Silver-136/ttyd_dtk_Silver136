#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "battle/battle_stage_object.h"
#include "system.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "tik.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"

f32 sinfd(f32 degrees);

static USER_FUNC(_wave_offset);
extern BattleUnitKind unit_boss_gesso;

BattleStageObjectData tik_btlobject_data[20];
BattleStageData tik_btlstage_data[4];
BattleUnitSetup btlparty_tik_tik_gesso[1];
BattleGroupSetup btlgrp_tik_tik_gesso;
BattleUnitSetup btlparty_tik_tik_01_01_off_1[4];
BattleGroupSetup btlgrp_tik_tik_01_01_off_1;
BattleUnitSetup btlparty_tik_tik_01_02_off_1[2];
BattleGroupSetup btlgrp_tik_tik_01_02_off_1;
BattleUnitSetup btlparty_tik_tik_04_01_off_1[1];
BattleGroupSetup btlgrp_tik_tik_04_01_off_1;
BattleUnitSetup btlparty_tik_tik_04_01_on_1[1];
BattleGroupSetup btlgrp_tik_tik_04_01_on_1;
BattleUnitSetup btlparty_tik_tik_04_01_on_2[2];
BattleGroupSetup btlgrp_tik_tik_04_01_on_2;
BattleUnitSetup btlparty_tik_tik_04_02_off_1[2];
BattleGroupSetup btlgrp_tik_tik_04_02_off_1;
BattleUnitSetup btlparty_tik_tik_04_02_on_1[2];
BattleGroupSetup btlgrp_tik_tik_04_02_on_1;
BattleUnitSetup btlparty_tik_tik_04_02_on_2[3];
BattleGroupSetup btlgrp_tik_tik_04_02_on_2;
BattleUnitSetup btlparty_tik_tik_04_03_off_1[2];
BattleGroupSetup btlgrp_tik_tik_04_03_off_1;
BattleUnitSetup btlparty_tik_tik_04_03_off_2[2];
BattleGroupSetup btlgrp_tik_tik_04_03_off_2;
BattleUnitSetup btlparty_tik_tik_04_03_off_3[2];
BattleGroupSetup btlgrp_tik_tik_04_03_off_3;
BattleUnitSetup btlparty_tik_tik_04_04_off_1[2];
BattleGroupSetup btlgrp_tik_tik_04_04_off_1;
BattleUnitSetup btlparty_tik_tik_04_04_off_2[2];
BattleGroupSetup btlgrp_tik_tik_04_04_off_2;
BattleUnitSetup btlparty_tik_tik_04_04_off_3[2];
BattleGroupSetup btlgrp_tik_tik_04_04_off_3;
BattleUnitSetup btlparty_tik_tik_07_01_off_1[3];
BattleGroupSetup btlgrp_tik_tik_07_01_off_1;
BattleUnitSetup btlparty_tik_tik_07_01_off_2[2];
BattleGroupSetup btlgrp_tik_tik_07_01_off_2;
BattleUnitSetup btlparty_tik_tik_07_02_off_1[2];
BattleGroupSetup btlgrp_tik_tik_07_02_off_1;
BattleUnitSetup btlparty_tik_tik_07_02_off_2[3];
BattleGroupSetup btlgrp_tik_tik_07_02_off_2;
BattleUnitSetup btlparty_tik_tik_07_03_off_1[2];
BattleGroupSetup btlgrp_tik_tik_07_03_off_1;
BattleUnitSetup btlparty_tik_tik_07_03_off_2[2];
BattleGroupSetup btlgrp_tik_tik_07_03_off_2;
BattleUnitSetup btlparty_tik_tik_07_03_off_3[2];
BattleGroupSetup btlgrp_tik_tik_07_03_off_3;
BattleUnitSetup btlparty_tik_tik_07_04_off_1[2];
BattleGroupSetup btlgrp_tik_tik_07_04_off_1;
BattleUnitSetup btlparty_tik_tik_07_04_off_2[2];
BattleGroupSetup btlgrp_tik_tik_07_04_off_2;
BattleUnitSetup btlparty_tik_tik_07_04_off_3[2];
BattleGroupSetup btlgrp_tik_tik_07_04_off_3;
BattleUnitSetup btlparty_tik_tik_15_01_off_1[1];
BattleGroupSetup btlgrp_tik_tik_15_01_off_1;
BattleUnitSetup btlparty_tik_tik_15_02_off_1[1];
BattleGroupSetup btlgrp_tik_tik_15_02_off_1;
BattleUnitSetup btlparty_tik_tik_15_03_off_1[1];
BattleGroupSetup btlgrp_tik_tik_15_03_off_1;
BattleUnitSetup btlparty_tik_tik_15_04_off_1[2];
BattleGroupSetup btlgrp_tik_tik_15_04_off_1;
BattleUnitSetup btlparty_tik_tik_15_04_off_2[2];
BattleGroupSetup btlgrp_tik_tik_15_04_off_2;
BattleUnitSetup btlparty_tik_tik_15_04_off_3[2];
BattleGroupSetup btlgrp_tik_tik_15_04_off_3;
BattleUnitSetup btlparty_tik_tik_15_05_off_1[2];
BattleGroupSetup btlgrp_tik_tik_15_05_off_1;
BattleUnitSetup btlparty_tik_tik_15_05_off_2[2];
BattleGroupSetup btlgrp_tik_tik_15_05_off_2;
BattleUnitSetup btlparty_tik_tik_15_05_off_3[2];
BattleGroupSetup btlgrp_tik_tik_15_05_off_3;
BattleUnitSetup btlparty_tik_tik_18_01_off_1[2];
BattleGroupSetup btlgrp_tik_tik_18_01_off_1;
BattleUnitSetup btlparty_tik_tik_18_01_off_2[3];
BattleGroupSetup btlgrp_tik_tik_18_01_off_2;
BattleUnitSetup btlparty_tik_tik_18_02_off_1[4];
BattleGroupSetup btlgrp_tik_tik_18_02_off_1;
BattleUnitSetup btlparty_tik_tik_18_02_off_2[2];
BattleGroupSetup btlgrp_tik_tik_18_02_off_2;
BattleUnitSetup btlparty_tik_tik_18_03_off_1[2];
BattleGroupSetup btlgrp_tik_tik_18_03_off_1;
BattleUnitSetup btlparty_tik_tik_18_03_off_2[2];
BattleGroupSetup btlgrp_tik_tik_18_03_off_2;
BattleUnitSetup btlparty_tik_tik_18_03_off_3[2];
BattleGroupSetup btlgrp_tik_tik_18_03_off_3;
BattleUnitSetup btlparty_tik_tik_18_04_off_1[2];
BattleGroupSetup btlgrp_tik_tik_18_04_off_1;
BattleUnitSetup btlparty_tik_tik_18_04_off_2[2];
BattleGroupSetup btlgrp_tik_tik_18_04_off_2;
BattleUnitSetup btlparty_tik_tik_18_04_off_3[2];
BattleGroupSetup btlgrp_tik_tik_18_04_off_3;
BattleUnitSetup btlparty_tik_tik_20_01_off_1[5];
BattleGroupSetup btlgrp_tik_tik_20_01_off_1;
BattleUnitSetup btlparty_tik_tik_20_02_off_1[4];
BattleGroupSetup btlgrp_tik_tik_20_02_off_1;
BattleUnitSetup btlparty_tik_tik_20_03_off_1[3];
BattleGroupSetup btlgrp_tik_tik_20_03_off_1;
BattleUnitSetup btlparty_tik_tik_20_04_off_1[4];
BattleGroupSetup btlgrp_tik_tik_20_04_off_1;
BattleUnitSetup btlparty_tik_tik_20_05_off_1[5];
BattleGroupSetup btlgrp_tik_tik_20_05_off_1;
BattleUnitSetup btlparty_tik_tik_20_06_off_1[5];
BattleGroupSetup btlgrp_tik_tik_20_06_off_1;
NpcBattleSelect btlgrpsel_tik_btlno_tik_test1_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_test2_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_test3_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_gesso_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_01_01_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_01_02_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_04_01_on[3];
NpcBattleSelect btlgrpsel_tik_btlno_tik_04_01_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_04_02_on[3];
NpcBattleSelect btlgrpsel_tik_btlno_tik_04_02_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_04_03_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_04_04_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_07_01_off[3];
NpcBattleSelect btlgrpsel_tik_btlno_tik_07_02_off[3];
NpcBattleSelect btlgrpsel_tik_btlno_tik_07_03_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_07_04_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_15_01_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_15_02_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_15_03_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_15_04_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_15_05_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_18_01_off[3];
NpcBattleSelect btlgrpsel_tik_btlno_tik_18_02_off[3];
NpcBattleSelect btlgrpsel_tik_btlno_tik_18_03_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_18_04_off[4];
NpcBattleSelect btlgrpsel_tik_btlno_tik_20_01_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_20_02_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_20_03_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_20_04_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_20_05_off[2];
NpcBattleSelect btlgrpsel_tik_btlno_tik_20_06_off[2];
NpcSetupBattleInfo btlsetup_tik_tbl[30];
NpcTerritoryInfo btl_tik_setup_no_tbl[30];
EvtWord bgset_tik_wave_event[59];

BattleStageObjectData tik_btlobject_data[] = {
    {"isi_2", 2, 0, {-150.0f, 0.0f, -23.0f}, 10, 50, {0}},
    {"isi_3", 2, 0, {-60.0f, 0.0f, -54.0f}, 20, 40, {0}},
    {"isi_4", 2, 0, {176.0f, 0.0f, -62.0f}, 30, 30, {0}},
    {"nami_0", 2, 2, {0}, 0, 60, {0}},
    {"nami_1", 2, 2, {0}, 0, 60, {0}},
    {"nami_2", 2, 2, {0}, 0, 60, {0}},
    {"nami_3", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"nami_0", 2, 2, {0}, 0, 60, {0}},
    {"nami_1", 2, 2, {0}, 0, 60, {0}},
    {"nami_2", 2, 2, {0}, 0, 60, {0}},
    {"nami_3", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B_0", 2, 2, {0}, 0, 60, {0}},
    {"jon_0", 2, 3, {0}, 0, 60, {0}},
    {"isi_1", 2, 1, {-202.0f, 0.0f, -17.0f}, 0, 60, {0}},
    {"isi_2", 2, 1, {200.0f, 0.0f, -30.0f}, 10, 50, {0}},
    {"A1", 2, 0, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"B_mati", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData tik_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_2", // mSpecificStageDataDir
        8, // mNumObjects
        tik_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_2", // mSpecificStageDataDir
        5, // mNumObjects
        &tik_btlobject_data[8], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_3", // mSpecificStageDataDir
        2, // mNumObjects
        &tik_btlobject_data[13], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_1", // mSpecificStageDataDir
        5, // mNumObjects
        &tik_btlobject_data[15], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_tik_tik_gesso[] = {
    {
        &unit_boss_gesso, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {122.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gesso, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_gesso = {
    1, // numEnemies
    btlparty_tik_tik_gesso, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_01_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_lecture_frankli_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_01_01_off_1 = {
    4, // numEnemies
    btlparty_tik_tik_01_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_01_02_off_1[] = {
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_01_02_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_01_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_01_off_1[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_01_off_1 = {
    1, // numEnemies
    btlparty_tik_tik_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_01_on_1[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_01_on_1 = {
    1, // numEnemies
    btlparty_tik_tik_04_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_01_on_2[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_01_on_2 = {
    2, // numEnemies
    btlparty_tik_tik_04_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_02_off_1[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_02_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_02_on_1[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_02_on_1 = {
    2, // numEnemies
    btlparty_tik_tik_04_02_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_02_on_2[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_02_on_2 = {
    3, // numEnemies
    btlparty_tik_tik_04_02_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_03_off_1[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_03_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_04_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_03_off_2[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_03_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_04_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_03_off_3[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_03_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_04_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_04_off_1[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_04_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_04_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_04_off_2[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_04_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_04_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_04_04_off_3[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_04_04_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_04_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_01_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_01_off_1 = {
    3, // numEnemies
    btlparty_tik_tik_07_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_01_off_2[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_01_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_07_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_02_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_02_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_07_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_02_off_2[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_02_off_2 = {
    3, // numEnemies
    btlparty_tik_tik_07_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_03_off_1[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_03_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_07_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_03_off_2[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_03_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_07_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_03_off_3[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_03_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_07_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_04_off_1[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_04_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_07_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_04_off_2[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_04_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_07_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_07_04_off_3[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_07_04_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_07_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_01_off_1 = {
    1, // numEnemies
    btlparty_tik_tik_15_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_02_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_02_off_1 = {
    1, // numEnemies
    btlparty_tik_tik_15_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_03_off_1[] = {
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_03_off_1 = {
    1, // numEnemies
    btlparty_tik_tik_15_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_04_off_1[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_04_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_15_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_04_off_2[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_04_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_15_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_04_off_3[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_04_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_15_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_05_off_1[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_05_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_15_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_05_off_2[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_05_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_15_05_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_15_05_off_3[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_15_05_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_15_05_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_01_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_01_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_18_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_01_off_2[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_01_off_2 = {
    3, // numEnemies
    btlparty_tik_tik_18_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_02_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_02_off_1 = {
    4, // numEnemies
    btlparty_tik_tik_18_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_02_off_2[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_02_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_18_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_03_off_1[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_03_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_18_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_03_off_2[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_03_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_18_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_03_off_3[] = {
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_03_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_18_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_04_off_1[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_04_off_1 = {
    2, // numEnemies
    btlparty_tik_tik_18_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_04_off_2[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_04_off_2 = {
    2, // numEnemies
    btlparty_tik_tik_18_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_18_04_off_3[] = {
    {
        &unit_togenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko, // itemDropTable
    },
    {
        &unit_kamec, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_18_04_off_3 = {
    2, // numEnemies
    btlparty_tik_tik_18_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_20_01_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_20_01_off_1 = {
    5, // numEnemies
    btlparty_tik_tik_20_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_20_02_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_20_02_off_1 = {
    4, // numEnemies
    btlparty_tik_tik_20_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_20_03_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_20_03_off_1 = {
    3, // numEnemies
    btlparty_tik_tik_20_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_20_04_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_20_04_off_1 = {
    4, // numEnemies
    btlparty_tik_tik_20_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_20_05_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_20_05_off_1 = {
    5, // numEnemies
    btlparty_tik_tik_20_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tik_tik_20_06_off_1[] = {
    {
        &unit_hennya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tik_tik_20_06_off_1 = {
    5, // numEnemies
    btlparty_tik_tik_20_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_test1_off[] = {
    {50, &btlgrp_tik_tik_01_01_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_test2_off[] = {
    {50, &btlgrp_tik_tik_01_01_off_1, tik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_test3_off[] = {
    {50, &btlgrp_tik_tik_01_01_off_1, &tik_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_gesso_off[] = {
    {50, &btlgrp_tik_tik_gesso, tik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_01_01_off[] = {
    {50, &btlgrp_tik_tik_01_01_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_01_02_off[] = {
    {50, &btlgrp_tik_tik_01_02_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_04_01_on[] = {
    {20, &btlgrp_tik_tik_04_01_on_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_04_01_on_2, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_04_01_off[] = {
    {20, &btlgrp_tik_tik_04_01_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_04_02_on[] = {
    {20, &btlgrp_tik_tik_04_02_on_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_04_02_on_2, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_04_02_off[] = {
    {20, &btlgrp_tik_tik_04_02_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_04_03_off[] = {
    {20, &btlgrp_tik_tik_04_03_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_04_03_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_04_03_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_04_04_off[] = {
    {20, &btlgrp_tik_tik_04_04_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_04_04_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_04_04_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_07_01_off[] = {
    {20, &btlgrp_tik_tik_07_01_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_07_01_off_2, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_07_02_off[] = {
    {20, &btlgrp_tik_tik_07_02_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_07_02_off_2, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_07_03_off[] = {
    {20, &btlgrp_tik_tik_07_03_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_07_03_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_07_03_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_07_04_off[] = {
    {20, &btlgrp_tik_tik_07_04_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_07_04_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_07_04_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_15_01_off[] = {
    {20, &btlgrp_tik_tik_15_01_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_15_02_off[] = {
    {20, &btlgrp_tik_tik_15_02_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_15_03_off[] = {
    {20, &btlgrp_tik_tik_15_03_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_15_04_off[] = {
    {20, &btlgrp_tik_tik_15_04_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_15_04_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_15_04_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_15_05_off[] = {
    {20, &btlgrp_tik_tik_15_05_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_15_05_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_15_05_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_18_01_off[] = {
    {20, &btlgrp_tik_tik_18_01_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_18_01_off_2, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_18_02_off[] = {
    {20, &btlgrp_tik_tik_18_02_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_18_02_off_2, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_18_03_off[] = {
    {20, &btlgrp_tik_tik_18_03_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_18_03_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_18_03_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_18_04_off[] = {
    {20, &btlgrp_tik_tik_18_04_off_1, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_18_04_off_2, &tik_btlstage_data[3]},
    {20, &btlgrp_tik_tik_18_04_off_3, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_20_01_off[] = {
    {20, &btlgrp_tik_tik_20_01_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_20_02_off[] = {
    {20, &btlgrp_tik_tik_20_02_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_20_03_off[] = {
    {20, &btlgrp_tik_tik_20_03_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_20_04_off[] = {
    {20, &btlgrp_tik_tik_20_04_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_20_05_off[] = {
    {20, &btlgrp_tik_tik_20_05_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_tik_btlno_tik_20_06_off[] = {
    {20, &btlgrp_tik_tik_20_06_off_1, &tik_btlstage_data[3]},
    {0},
};

NpcSetupBattleInfo btlsetup_tik_tbl[] = {
    {
        /*"マップstg_00_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x30\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_00_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x30\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_00_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x30\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik12ゲッソー"*/"\x74\x69\x6B\x31\x32\x83\x51\x83\x62\x83\x5C\x81\x5B", // name
        "tik_02", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_gesso_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"tik01クリボー３人衆"*/"\x74\x69\x6B\x30\x31\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x52\x90\x6C\x8F\x4F", // name
        "tik_01", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_01_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik01トゲクリボー"*/"\x74\x69\x6B\x30\x31\x83\x67\x83\x51\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_01_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik04ヒンニャー1"*/"\x74\x69\x6B\x30\x34\x83\x71\x83\x93\x83\x6A\x83\x83\x81\x5B\x31", // name
        NULL, 0, // mapName..fixedItem
        GSWF(1246), // swFlag
        btlgrpsel_tik_btlno_tik_04_01_on, // altSelect
        btlgrpsel_tik_btlno_tik_04_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik04ヒンニャー2"*/"\x74\x69\x6B\x30\x34\x83\x71\x83\x93\x83\x6A\x83\x83\x81\x5B\x32", // name
        NULL, 0, // mapName..fixedItem
        GSWF(1246), // swFlag
        btlgrpsel_tik_btlno_tik_04_02_on, // altSelect
        btlgrpsel_tik_btlno_tik_04_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik04カメック"*/"\x74\x69\x6B\x30\x34\x83\x4A\x83\x81\x83\x62\x83\x4E", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_04_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik04トゲノコ"*/"\x74\x69\x6B\x30\x34\x83\x67\x83\x51\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_04_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik07ハンニャー1"*/"\x74\x69\x6B\x30\x37\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_07_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik07ハンニャー2"*/"\x74\x69\x6B\x30\x37\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_07_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik07トゲノコ"*/"\x74\x69\x6B\x30\x37\x83\x67\x83\x51\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_07_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik07ハンマーブロス"*/"\x74\x69\x6B\x30\x37\x83\x6E\x83\x93\x83\x7D\x81\x5B\x83\x75\x83\x8D\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_07_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 10}, {10, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik15クリボー"*/"\x74\x69\x6B\x31\x35\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_15_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik15パタクリボー"*/"\x74\x69\x6B\x31\x35\x83\x70\x83\x5E\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_15_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik15トゲクリボー"*/"\x74\x69\x6B\x31\x35\x83\x67\x83\x51\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_15_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik15ハンマーブロス"*/"\x74\x69\x6B\x31\x35\x83\x6E\x83\x93\x83\x7D\x81\x5B\x83\x75\x83\x8D\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_15_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 10}, {10, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik15カメック"*/"\x74\x69\x6B\x31\x35\x83\x4A\x83\x81\x83\x62\x83\x4E", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_15_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik18ハンニャー1"*/"\x74\x69\x6B\x31\x38\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_18_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik18ハンニャー2"*/"\x74\x69\x6B\x31\x38\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_18_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik18カメック"*/"\x74\x69\x6B\x31\x38\x83\x4A\x83\x81\x83\x62\x83\x4E", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_18_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik18トゲノコ"*/"\x74\x69\x6B\x31\x38\x83\x67\x83\x51\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_18_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik20ハンニャー"*/"\x74\x69\x6B\x32\x30\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_20_01_off, // select
        0, 0, // battleFlags..field_1C
        {{2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik20ハンニャー"*/"\x74\x69\x6B\x32\x30\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_20_02_off, // select
        0, 0, // battleFlags..field_1C
        {{2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik20ハンニャー"*/"\x74\x69\x6B\x32\x30\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_20_03_off, // select
        0, 0, // battleFlags..field_1C
        {{2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik20ハンニャー"*/"\x74\x69\x6B\x32\x30\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_20_04_off, // select
        0, 0, // battleFlags..field_1C
        {{2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik20ハンニャー"*/"\x74\x69\x6B\x32\x30\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_20_05_off, // select
        0, 0, // battleFlags..field_1C
        {{2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"tik20ヘンニャー"*/"\x74\x69\x6B\x32\x30\x83\x77\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tik_btlno_tik_20_06_off, // select
        0, 0, // battleFlags..field_1C
        {{2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_tik_setup_no_tbl[] = {
    {"BTLNO_TIK_TEST1", 0},
    {"BTLNO_TIK_TEST2", 1},
    {"BTLNO_TIK_TEST3", 2},
    {"BTLNO_TIK_GESSO", 3},
    {"BTLNO_TIK_01_01", 4},
    {"BTLNO_TIK_01_02", 5},
    {"BTLNO_TIK_04_01", 6},
    {"BTLNO_TIK_04_02", 7},
    {"BTLNO_TIK_04_03", 8},
    {"BTLNO_TIK_04_04", 9},
    {"BTLNO_TIK_07_01", 10},
    {"BTLNO_TIK_07_02", 11},
    {"BTLNO_TIK_07_03", 12},
    {"BTLNO_TIK_07_04", 13},
    {"BTLNO_TIK_15_01", 14},
    {"BTLNO_TIK_15_02", 15},
    {"BTLNO_TIK_15_03", 16},
    {"BTLNO_TIK_15_04", 17},
    {"BTLNO_TIK_15_05", 18},
    {"BTLNO_TIK_18_01", 19},
    {"BTLNO_TIK_18_02", 20},
    {"BTLNO_TIK_18_03", 21},
    {"BTLNO_TIK_18_04", 22},
    {"BTLNO_TIK_20_01", 23},
    {"BTLNO_TIK_20_02", 24},
    {"BTLNO_TIK_20_03", 25},
    {"BTLNO_TIK_20_04", 26},
    {"BTLNO_TIK_20_05", 27},
    {"BTLNO_TIK_20_06", 28},
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

EVT_BEGIN(bgset_tik_wave_event)
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

