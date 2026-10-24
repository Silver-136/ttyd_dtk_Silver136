#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "battle/battle_stage_object.h"
#include "system.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "muj.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"

f32 sinfd(f32 degrees);

static USER_FUNC(_wave_offset);
USER_FUNC(evt_eff64);
extern BattleUnitKind unit_boss_cortez;
extern BattleUnitKind unit_boss_kanbu3;

f32 fd_battle_database_muj_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_muj_neg_one_382;
f32 fd_battle_database_muj___local_PI_383;
f32 fd_battle_database_muj___local_3PIO2_385;
f32 fd_battle_database_muj_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_muj___local_PIO2_384;
f32 fd_battle_database_muj_one_381;
f32 fd_battle_database_muj_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_muj_neg_one_365;
f32 fd_battle_database_muj___local_PI_366;
f32 fd_battle_database_muj___local_3PIO2_368;
f32 fd_battle_database_muj_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_muj___local_PIO2_367;
f32 fd_battle_database_muj_one_364;
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

f32 fd_battle_database_muj_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_muj_neg_one_382 = -1.0f;

f32 fd_battle_database_muj___local_PI_383 = 3.1415927f;

f32 fd_battle_database_muj___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_muj_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_muj___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_muj_one_381 = 1.0f;

f32 fd_battle_database_muj_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_muj_neg_one_365 = -1.0f;

f32 fd_battle_database_muj___local_PI_366 = 3.1415927f;

f32 fd_battle_database_muj___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_muj_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_muj___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_muj_one_364 = 1.0f;

BattleStageObjectData muj_btlobject_data[] = {
    {"A1", 2, 0, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"hata", 2, 1, {178.0f, 0.0f, -49.0f}, 0, 60, {0}},
    {"iwa", 2, 1, {-183.0f, 0.0f, -28.0f}, 10, 50, {0}},
    {"zai", 2, 1, {126.0f, 0.0f, -26.0f}, 20, 40, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"A1", 2, 0, {0.0f, 0.0f, -56.0f}, 0, 60, {0}},
    {"kusa_mae", 2, 1, {0.0f, 0.0f, 70.0f}, 0, 60, {0}},
    {"kusa_usiro", 2, 1, {0.0f, 0.0f, -44.0f}, 10, 50, {0}},
    {"kusa_01", 2, 1, {-79.0f, 0.0f, -28.0f}, 20, 40, {0}},
    {"kusa_02", 2, 1, {128.0f, 0.0f, -33.0f}, 30, 30, {0}},
    {"necco", 2, 1, {0.0f, 0.0f, -34.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"takara_01", 2, 1, {111.0f, 0.0f, -69.0f}, 0, 60, {0}},
    {"takara_02", 2, 1, {-87.0f, 0.0f, -72.0f}, 10, 50, {0}},
    {"takara_03", 2, 1, {136.0f, 0.0f, 57.0f}, 20, 40, {0}},
    {"takara_04", 2, 1, {-182.0f, 0.0f, 57.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C_kabe", 2, 3, {0}, 0, 60, {0}},
    {"nami_00", 2, 3, {0}, 0, 60, {0}},
    {"nami_01", 2, 3, {0}, 0, 60, {0}},
    {"A1", 2, 0, {107.0f, 0.0f, -53.0f}, 0, 60, {0}},
    {"S_mae_l", 2, 1, {-90.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_mae_r", 2, 1, {84.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_oku_l", 2, 1, {-35.0f, 0.0f, -42.0f}, 0, 60, {0}},
    {"S_oku_r", 2, 1, {35.0f, 0.0f, -42.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
};

BattleStageData muj_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_1", // mSpecificStageDataDir
        6, // mNumObjects
        muj_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_muj_sun_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_2", // mSpecificStageDataDir
        8, // mNumObjects
        &muj_btlobject_data[6], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_muj_sun_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_5", // mSpecificStageDataDir
        8, // mNumObjects
        &muj_btlobject_data[14], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_05_0", // mSpecificStageDataDir
        9, // mNumObjects
        &muj_btlobject_data[22], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_muj_sun_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_muj_muj_cortez[] = {
    {
        &unit_boss_cortez, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_kanbu[] = {
    {
        &unit_boss_gundan_zako_group1, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {80.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_boss_kanbu3, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_01_01[] = {
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_05_01[] = {
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_05_02[] = {
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_hermos, // mUnitKindParams
        1, // alliance
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_00_01[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_01[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_02[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_03[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_01_off_04[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_04_off_01[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_04_off_02[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_04_off_03[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_07_off_01[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_07_off_02[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_02_07_off_03[] = {
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_green_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_01_off_01[] = {
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_01_off_02[] = {
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_01_off_03[] = {
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_02_off_01[] = {
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_02_off_02[] = {
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_02_off_03[] = {
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_04_off_01[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_04_off_02[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_muj_muj_03_04_off_03[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_poison_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_poison_pakkun, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
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

NpcSetupBattleInfo btlsetup_muj_tbl[] = {
    {
        /*"マップstg_05_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_05_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_05_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_05_5"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x35\x5F\x35", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_test4_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"コルテス"*/"\x83\x52\x83\x8B\x83\x65\x83\x58", // name
        "muj_12", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_cortez_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG5_KORTESS1", // bgmName
    },
    {
        /*"幹部その３"*/"\x8A\xB2\x95\x94\x82\xBB\x82\xCC\x82\x52", // name
        "muj_00", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_kanbu_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_3RDLEADER1", // bgmName
    },
    {
        /*"MUJ01おんりょう１"*/"\x4D\x55\x4A\x30\x31\x82\xA8\x82\xF1\x82\xE8\x82\xE5\x82\xA4\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_01_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"MUJ01おんりょう２"*/"\x4D\x55\x4A\x30\x31\x82\xA8\x82\xF1\x82\xE8\x82\xE5\x82\xA4\x82\x51", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_05_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"MUJ01おんりょう３"*/"\x4D\x55\x4A\x30\x31\x82\xA8\x82\xF1\x82\xE8\x82\xE5\x82\xA4\x82\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_05_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"MUJ02グリーンチョロボン"*/"\x4D\x55\x4A\x30\x32\x83\x4F\x83\x8A\x81\x5B\x83\x93\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_02_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"MUJ02フラワーチョロボン"*/"\x4D\x55\x4A\x30\x32\x83\x74\x83\x89\x83\x8F\x81\x5B\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"MUJ02グリーンチョロボン"*/"\x4D\x55\x4A\x30\x32\x83\x4F\x83\x8A\x81\x5B\x83\x93\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_02_07_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"MUJ03ポイズンパックン"*/"\x4D\x55\x4A\x30\x33\x83\x7C\x83\x43\x83\x59\x83\x93\x83\x70\x83\x62\x83\x4E\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"MUJ03ポイズンパックン"*/"\x4D\x55\x4A\x30\x33\x83\x7C\x83\x43\x83\x59\x83\x93\x83\x70\x83\x62\x83\x4E\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_03_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"MUJ03フラワーチョロボン"*/"\x4D\x55\x4A\x30\x33\x83\x74\x83\x89\x83\x8F\x81\x5B\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_muj_btlno_muj_03_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
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

EVT_BEGIN(bgset_muj_sun_event)
    CALL(evt_eff64, STRING("sunlight_bt"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

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

