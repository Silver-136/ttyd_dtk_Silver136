#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "tou2.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(tou_gamen_screen_tev_init);
extern s32 tou_gamen_screen_tev[];
extern BattleUnitKind unit_jyugem;

BattleStageObjectData tou_btlobject_data[14];
BattleStageData tou_btlstage_data[2];
BattleUnitSetup btlparty_tou_tou_rank_19[5];
BattleGroupSetup btlgrp_tou_tou_rank_19;
BattleUnitSetup btlparty_tou_tou_rank_18[3];
BattleGroupSetup btlgrp_tou_tou_rank_18;
BattleUnitSetup btlparty_tou_tou_rank_17[3];
BattleGroupSetup btlgrp_tou_tou_rank_17;
BattleUnitSetup btlparty_tou_tou_rank_16[3];
BattleGroupSetup btlgrp_tou_tou_rank_16;
BattleUnitSetup btlparty_tou_tou_rank_15[3];
BattleGroupSetup btlgrp_tou_tou_rank_15;
BattleUnitSetup btlparty_tou_tou_rank_14[3];
BattleGroupSetup btlgrp_tou_tou_rank_14;
BattleUnitSetup btlparty_tou_tou_rank_13[3];
BattleGroupSetup btlgrp_tou_tou_rank_13;
BattleUnitSetup btlparty_tou_tou_rank_12[4];
BattleGroupSetup btlgrp_tou_tou_rank_12;
BattleUnitSetup btlparty_tou_tou_rank_11[4];
BattleGroupSetup btlgrp_tou_tou_rank_11;
BattleUnitSetup btlparty_tou_tou_rank_10[2];
BattleGroupSetup btlgrp_tou_tou_rank_10;
BattleUnitSetup btlparty_tou_tou_rank_9[2];
BattleGroupSetup btlgrp_tou_tou_rank_9;
BattleUnitSetup btlparty_tou_tou_rank_8[2];
BattleGroupSetup btlgrp_tou_tou_rank_8;
BattleUnitSetup btlparty_tou_tou_rank_7[3];
BattleGroupSetup btlgrp_tou_tou_rank_7;
BattleUnitSetup btlparty_tou_tou_rank_6[3];
BattleGroupSetup btlgrp_tou_tou_rank_6;
BattleUnitSetup btlparty_tou_tou_rank_5[3];
BattleGroupSetup btlgrp_tou_tou_rank_5;
BattleUnitSetup btlparty_tou_tou_rank_4[1];
BattleGroupSetup btlgrp_tou_tou_rank_4;
BattleUnitSetup btlparty_tou_tou_rank_3[3];
BattleGroupSetup btlgrp_tou_tou_rank_3;
BattleUnitSetup btlparty_tou_tou_rank_2[2];
BattleGroupSetup btlgrp_tou_tou_rank_2;
BattleUnitSetup btlparty_tou_tou_rank_1[1];
BattleGroupSetup btlgrp_tou_tou_rank_1;
BattleUnitSetup btlparty_tou_tou_add_1[4];
BattleGroupSetup btlgrp_tou_tou_add_1;
BattleUnitSetup btlparty_tou_tou_add_2[4];
BattleGroupSetup btlgrp_tou_tou_add_2;
BattleUnitSetup btlparty_tou_tou_boss[1];
BattleGroupSetup btlgrp_tou_tou_boss;
BattleUnitSetup btlparty_tou_tou_champ[1];
BattleGroupSetup btlgrp_tou_tou_champ;
BattleUnitSetup btlparty_tou_tou_koopa[1];
BattleGroupSetup btlgrp_tou_tou_koopa;
NpcBattleSelect btlgrpsel_tou_btlno_tou_test1_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_01_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_02_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_03_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_04_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_05_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_06_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_07_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_08_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_09_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_10_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_11_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_12_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_13_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_14_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_15_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_16_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_17_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_18_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_19_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_20_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_21_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_22_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_23_off[2];
NpcBattleSelect btlgrpsel_tou_btlno_tou_01_24_off[2];
NpcSetupBattleInfo btlsetup_tou_tbl[26];
NpcTerritoryInfo btl_tou_setup_no_tbl[26];
EvtWord bgset_tou_screen_evt[22];

BattleStageObjectData tou_btlobject_data[] = {
    {"gamen", 2, 0, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"etc", 2, 0, {0.0f, 0.0f, -54.0f}, 10, 50, {0}},
    {"side__L", 2, 0, {-137.0f, 0.0f, -40.0f}, 20, 40, {0}},
    {"side__R", 2, 0, {137.0f, 0.0f, -40.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"gamen", 2, 0, {0.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"etc", 2, 0, {0.0f, 0.0f, -54.0f}, 10, 50, {0}},
    {"side__L", 2, 0, {-137.0f, 0.0f, -40.0f}, 20, 40, {0}},
    {"side__R", 2, 0, {137.0f, 0.0f, -40.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
};

BattleStageData tou_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_03_0", // mSpecificStageDataDir
        6, // mNumObjects
        tou_btlobject_data, // mObjects
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
        "bti_01", // mGlobalStageDataDir
        "stg_03_0", // mSpecificStageDataDir
        8, // mNumObjects
        &tou_btlobject_data[6], // mObjects
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

BattleUnitSetup btlparty_tou_tou_rank_19[] = {
    {
        &unit_kurikuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
    {
        &unit_kurikuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurikuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_19 = {
    5, // numEnemies
    btlparty_tou_tou_rank_19, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_18[] = {
    {
        &unit_nokonoko_fighter, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko_fighter, // itemDropTable
    },
    {
        &unit_nokonoko_fighter, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko_fighter, // itemDropTable
    },
    {
        &unit_patapata_fighter, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_patapata_fighter, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_18 = {
    3, // numEnemies
    btlparty_tou_tou_rank_18, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_17[] = {
    {
        &unit_sambo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_17 = {
    3, // numEnemies
    btlparty_tou_tou_rank_17, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_16[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_16 = {
    3, // numEnemies
    btlparty_tou_tou_rank_16, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_15[] = {
    {
        &unit_togezo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_togezo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_jyugem, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_15 = {
    3, // numEnemies
    btlparty_tou_tou_rank_15, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_14[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togezo, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togezo, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_togezo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_14 = {
    3, // numEnemies
    btlparty_tou_tou_rank_14, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_13[] = {
    {
        &unit_hyper_sinnosuke, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinnosuke, // itemDropTable
    },
    {
        &unit_hyper_sinnosuke, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinnosuke, // itemDropTable
    },
    {
        &unit_hyper_sinnosuke, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_hyper_sinnosuke, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_13 = {
    3, // numEnemies
    btlparty_tou_tou_rank_13, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_12[] = {
    {
        &unit_bomhei, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_12 = {
    4, // numEnemies
    btlparty_tou_tou_rank_12, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_11[] = {
    {
        &unit_borodo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo_king, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo_king, // itemDropTable
    },
    {
        &unit_borodo_king, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo_king, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_11 = {
    4, // numEnemies
    btlparty_tou_tou_rank_11, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_10[] = {
    {
        &unit_iron_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {80.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {50, 0, 0, 0}, // unitWork
        battle_item_iron_sinemon, // itemDropTable
    },
    {
        &unit_iron_sinemon2, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {130, 0, 0, 0}, // unitWork
        battle_item_iron_sinemon2, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_10 = {
    2, // numEnemies
    btlparty_tou_tou_rank_10, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_9[] = {
    {
        &unit_crimson_togemet, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togemet, // itemDropTable
    },
    {
        &unit_crimson_togemet, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_9 = {
    2, // numEnemies
    btlparty_tou_tou_rank_9, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_8[] = {
    {
        &unit_togedaruma, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_togedaruma, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_8 = {
    2, // numEnemies
    btlparty_tou_tou_rank_8, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_7[] = {
    {
        &unit_ura_noko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_pata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_ura_pata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_7 = {
    3, // numEnemies
    btlparty_tou_tou_rank_7, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_6[] = {
    {
        &unit_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_green_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kamec, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_6 = {
    3, // numEnemies
    btlparty_tou_tou_rank_6, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_5[] = {
    {
        &unit_kamec_red, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hammer_bros, // itemDropTable
    },
    {
        &unit_kamec_white, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_fire_bros, // itemDropTable
    },
    {
        &unit_kamec_green, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_boomerang_bros, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_5 = {
    3, // numEnemies
    btlparty_tou_tou_rank_5, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_4[] = {
    {
        &unit_dark_keeper, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_4 = {
    1, // numEnemies
    btlparty_tou_tou_rank_4, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_3[] = {
    {
        &unit_hammer_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_fire_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_boomerang_bros, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_3 = {
    3, // numEnemies
    btlparty_tou_tou_rank_3, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_2[] = {
    {
        &unit_burst_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_burst_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_2 = {
    2, // numEnemies
    btlparty_tou_tou_rank_2, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_rank_1[] = {
    {
        &unit_togenoko_ace, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_rank_1 = {
    1, // numEnemies
    btlparty_tou_tou_rank_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_add_1[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_add_1 = {
    4, // numEnemies
    btlparty_tou_tou_add_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_add_2[] = {
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monban, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_add_2 = {
    4, // numEnemies
    btlparty_tou_tou_add_2, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_boss[] = {
    {
        &unit_boss_macho_gance, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_macho_gance, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_boss = {
    1, // numEnemies
    btlparty_tou_tou_boss, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_champ[] = {
    {
        &unit_boss_champion, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_champion, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_champ = {
    1, // numEnemies
    btlparty_tou_tou_champ, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_tou_tou_koopa[] = {
    {
        &unit_boss_koopa_tou, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_tou_tou_koopa = {
    1, // numEnemies
    btlparty_tou_tou_koopa, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_test1_off[] = {
    {50, &btlgrp_tou_tou_rank_19, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_01_off[] = {
    {50, &btlgrp_tou_tou_rank_19, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_02_off[] = {
    {50, &btlgrp_tou_tou_rank_18, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_03_off[] = {
    {50, &btlgrp_tou_tou_rank_17, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_04_off[] = {
    {50, &btlgrp_tou_tou_rank_16, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_05_off[] = {
    {50, &btlgrp_tou_tou_rank_15, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_06_off[] = {
    {50, &btlgrp_tou_tou_rank_14, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_07_off[] = {
    {50, &btlgrp_tou_tou_rank_13, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_08_off[] = {
    {50, &btlgrp_tou_tou_rank_12, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_09_off[] = {
    {50, &btlgrp_tou_tou_rank_11, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_10_off[] = {
    {50, &btlgrp_tou_tou_rank_10, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_11_off[] = {
    {50, &btlgrp_tou_tou_rank_9, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_12_off[] = {
    {50, &btlgrp_tou_tou_rank_8, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_13_off[] = {
    {50, &btlgrp_tou_tou_rank_7, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_14_off[] = {
    {50, &btlgrp_tou_tou_rank_6, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_15_off[] = {
    {50, &btlgrp_tou_tou_rank_5, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_16_off[] = {
    {50, &btlgrp_tou_tou_rank_4, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_17_off[] = {
    {50, &btlgrp_tou_tou_rank_3, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_18_off[] = {
    {50, &btlgrp_tou_tou_rank_2, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_19_off[] = {
    {50, &btlgrp_tou_tou_rank_1, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_20_off[] = {
    {50, &btlgrp_tou_tou_add_1, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_21_off[] = {
    {50, &btlgrp_tou_tou_add_2, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_22_off[] = {
    {50, &btlgrp_tou_tou_champ, &tou_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_23_off[] = {
    {50, &btlgrp_tou_tou_boss, tou_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_tou_btlno_tou_01_24_off[] = {
    {50, &btlgrp_tou_tou_koopa, tou_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_tou_tbl[] = {
    {
        /*"マップstg_03_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x33\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"クリクリブラザーズ"*/"\x83\x4E\x83\x8A\x83\x4E\x83\x8A\x83\x75\x83\x89\x83\x55\x81\x5B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_01_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"コウラーズ"*/"\x83\x52\x83\x45\x83\x89\x81\x5B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_02_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"サンボ三兄弟"*/"\x83\x54\x83\x93\x83\x7B\x8E\x4F\x8C\x5A\x92\xED", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_03_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ボンバーズ"*/"\x83\x7B\x83\x93\x83\x6F\x81\x5B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_04_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"クモーンズ"*/"\x83\x4E\x83\x82\x81\x5B\x83\x93\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_05_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ジュゲムとイタい仲間たち"*/"\x83\x57\x83\x85\x83\x51\x83\x80\x82\xC6\x83\x43\x83\x5E\x82\xA2\x92\x87\x8A\xD4\x82\xBD\x82\xBF", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_06_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ハイパーズ"*/"\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_07_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ボロドーズ"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_08_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"クッパぐんだんズ"*/"\x83\x4E\x83\x62\x83\x70\x82\xAE\x82\xF1\x82\xBE\x82\xF1\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_09_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"こうてつウォリアーズ"*/"\x82\xB1\x82\xA4\x82\xC4\x82\xC2\x83\x45\x83\x48\x83\x8A\x83\x41\x81\x5B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_10_off, // select
        0x20000040, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"ザ・トゲトゲン"*/"\x83\x55\x81\x45\x83\x67\x83\x51\x83\x67\x83\x51\x83\x93", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_11_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミトゲダルマー"*/"\x83\x84\x83\x7E\x83\x67\x83\x51\x83\x5F\x83\x8B\x83\x7D\x81\x5B", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_12_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ザ・ウラウラン"*/"\x83\x55\x81\x45\x83\x45\x83\x89\x83\x45\x83\x89\x83\x93", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_13_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"レッドデビルズ"*/"\x83\x8C\x83\x62\x83\x68\x83\x66\x83\x72\x83\x8B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_14_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"トリプルブロス"*/"\x83\x67\x83\x8A\x83\x76\x83\x8B\x83\x75\x83\x8D\x83\x58", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_15_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ザ・マホーンズ"*/"\x83\x55\x81\x45\x83\x7D\x83\x7A\x81\x5B\x83\x93\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_16_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"キラーボムヘイズ"*/"\x83\x4C\x83\x89\x81\x5B\x83\x7B\x83\x80\x83\x77\x83\x43\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_17_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"バーストワンワン"*/"\x83\x6F\x81\x5B\x83\x58\x83\x67\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_18_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"トゲノコエース"*/"\x83\x67\x83\x51\x83\x6D\x83\x52\x83\x47\x81\x5B\x83\x58", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_19_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ナイトウィングス"*/"\x83\x69\x83\x43\x83\x67\x83\x45\x83\x42\x83\x93\x83\x4F\x83\x58", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_20_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"スネイルズ"*/"\x83\x58\x83\x6C\x83\x43\x83\x8B\x83\x59", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_21_off, // select
        0x40, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ブレッドハート"*/"\x83\x75\x83\x8C\x83\x62\x83\x68\x83\x6E\x81\x5B\x83\x67", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_22_off, // select
        0x20000040, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_CHAMP1", // bgmName
    },
    {
        /*"マッチョガンス"*/"\x83\x7D\x83\x62\x83\x60\x83\x87\x83\x4B\x83\x93\x83\x58", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_23_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_GANCE1", // bgmName
    },
    {
        /*"乱入クッパ"*/"\x97\x90\x93\xFC\x83\x4E\x83\x62\x83\x70", // name
        "tou_03", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_tou_btlno_tou_01_24_off, // select
#ifdef VERSION_G8ME01
        0x20000040, // battleFlags
#else
        0x20000000, // battleFlags
#endif
        0, // field_1C
        {{0}, {0}, {0}, {0}, {4, 4}, {4, 4}, {0}, {0}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
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

EVT_BEGIN(bgset_tou_screen_evt)
    CALL(tou_gamen_screen_tev_init)
    WAIT_FRAMES(1)
    CALL(evt_map_set_tevcallback, 8, PTR(tou_gamen_screen_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gamen"), 134217728)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gamen"), 256)
    RETURN
EVT_END()

