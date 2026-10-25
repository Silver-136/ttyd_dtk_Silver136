#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"

#include "driver/effdrv.h"
#include "battle/battle.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "las.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"

EffectEntry* hi_fire_eff[6];
s8 hi_fire_end_flag[4];
EffectEntry* effFireEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 lifetime);
void mapObjGetPos(const char* name, Vec* pos);

USER_FUNC(_gear_rotate);
USER_FUNC(check_battle_end);
USER_FUNC(hi_fire_end);
USER_FUNC(hi_fire_main);
extern BattleUnitKind unit_black_karon;
extern BattleUnitKind unit_boss_batten_leader;
extern BattleUnitKind unit_boss_koopa;
extern BattleUnitKind unit_boss_rampell_las;
extern BattleUnitKind unit_red_honenoko;
extern BattleUnitKind unit_super_killer_cannon;

BattleStageObjectData las_btlobject_data[38];
BattleStageData las_btlstage_data[10];
BattleUnitSetup btlparty_las_las_05_01[5];
BattleGroupSetup btlgrp_las_las_05_01;
BattleUnitSetup btlparty_las_las_17_01_off_1[1];
BattleGroupSetup btlgrp_las_las_17_01_off_1;
BattleUnitSetup btlparty_las_las_17_02_off_1[1];
BattleGroupSetup btlgrp_las_las_17_02_off_1;
BattleUnitSetup btlparty_las_las_17_03_off_1[1];
BattleGroupSetup btlgrp_las_las_17_03_off_1;
BattleUnitSetup btlparty_las_las_17_04_off_1[1];
BattleGroupSetup btlgrp_las_las_17_04_off_1;
BattleUnitSetup btlparty_las_las_09_rampell[1];
BattleGroupSetup btlgrp_las_las_09_rampell;
BattleUnitSetup btlparty_las_las_bunbaba[1];
BattleGroupSetup btlgrp_las_las_bunbaba;
BattleUnitSetup btlparty_las_las_28_batten_leader[1];
BattleGroupSetup btlgrp_las_las_28_batten_leader;
BattleUnitSetup btlparty_las_las_28_koopa[1];
BattleGroupSetup btlgrp_las_las_28_koopa;
BattleUnitSetup btlparty_las_las_29_black_peach_1[1];
BattleGroupSetup btlgrp_las_las_29_black_peach_1;
BattleUnitSetup btlparty_las_las_29_black_peach_2[1];
BattleGroupSetup btlgrp_las_las_29_black_peach_2;
BattleUnitSetup btlparty_las_las_01_01_off_1[2];
BattleGroupSetup btlgrp_las_las_01_01_off_1;
BattleUnitSetup btlparty_las_las_01_02_off_1[3];
BattleGroupSetup btlgrp_las_las_01_02_off_1;
BattleUnitSetup btlparty_las_las_01_03_off_1[4];
BattleGroupSetup btlgrp_las_las_01_03_off_1;
BattleUnitSetup btlparty_las_las_01_01_off_2[2];
BattleGroupSetup btlgrp_las_las_01_01_off_2;
BattleUnitSetup btlparty_las_las_01_02_off_2[3];
BattleGroupSetup btlgrp_las_las_01_02_off_2;
BattleUnitSetup btlparty_las_las_01_03_off_2[4];
BattleGroupSetup btlgrp_las_las_01_03_off_2;
BattleUnitSetup btlparty_las_las_02_01_off_1[2];
BattleGroupSetup btlgrp_las_las_02_01_off_1;
BattleUnitSetup btlparty_las_las_02_02_off_1[3];
BattleGroupSetup btlgrp_las_las_02_02_off_1;
BattleUnitSetup btlparty_las_las_02_03_off_1[3];
BattleGroupSetup btlgrp_las_las_02_03_off_1;
BattleUnitSetup btlparty_las_las_02_04_off_1[4];
BattleGroupSetup btlgrp_las_las_02_04_off_1;
BattleUnitSetup btlparty_las_las_02_05_off_1[2];
BattleGroupSetup btlgrp_las_las_02_05_off_1;
BattleUnitSetup btlparty_las_las_02_06_off_1[2];
BattleGroupSetup btlgrp_las_las_02_06_off_1;
BattleUnitSetup btlparty_las_las_02_99_off_1[2];
BattleGroupSetup btlgrp_las_las_02_99_off_1;
BattleUnitSetup btlparty_las_las_03_01_off_1[1];
BattleGroupSetup btlgrp_las_las_03_01_off_1;
BattleUnitSetup btlparty_las_las_03_02_off_1[1];
BattleGroupSetup btlgrp_las_las_03_02_off_1;
BattleUnitSetup btlparty_las_las_03_03_off_1[1];
BattleGroupSetup btlgrp_las_las_03_03_off_1;
BattleUnitSetup btlparty_las_las_03_01_off_2[1];
BattleGroupSetup btlgrp_las_las_03_01_off_2;
BattleUnitSetup btlparty_las_las_03_02_off_2[1];
BattleGroupSetup btlgrp_las_las_03_02_off_2;
BattleUnitSetup btlparty_las_las_03_03_off_2[1];
BattleGroupSetup btlgrp_las_las_03_03_off_2;
BattleUnitSetup btlparty_las_las_04_01_off_1[1];
BattleGroupSetup btlgrp_las_las_04_01_off_1;
BattleUnitSetup btlparty_las_las_06_01_off_1[2];
BattleGroupSetup btlgrp_las_las_06_01_off_1;
BattleUnitSetup btlparty_las_las_06_02_off_1[2];
BattleGroupSetup btlgrp_las_las_06_02_off_1;
BattleUnitSetup btlparty_las_las_06_03_off_1[2];
BattleGroupSetup btlgrp_las_las_06_03_off_1;
BattleUnitSetup btlparty_las_las_06_04_off_1[4];
BattleGroupSetup btlgrp_las_las_06_04_off_1;
BattleUnitSetup btlparty_las_las_06_05_off_1[2];
BattleGroupSetup btlgrp_las_las_06_05_off_1;
BattleUnitSetup btlparty_las_las_06_06_off_1[2];
BattleGroupSetup btlgrp_las_las_06_06_off_1;
BattleUnitSetup btlparty_las_las_06_99_off_1[2];
BattleGroupSetup btlgrp_las_las_06_99_off_1;
BattleUnitSetup btlparty_las_las_07_01_off_1[3];
BattleGroupSetup btlgrp_las_las_07_01_off_1;
BattleUnitSetup btlparty_las_las_07_02_off_1[3];
BattleGroupSetup btlgrp_las_las_07_02_off_1;
BattleUnitSetup btlparty_las_las_07_03_off_1[3];
BattleGroupSetup btlgrp_las_las_07_03_off_1;
BattleUnitSetup btlparty_las_las_09_01_off_1[2];
BattleGroupSetup btlgrp_las_las_09_01_off_1;
BattleUnitSetup btlparty_las_las_09_02_off_1[3];
BattleGroupSetup btlgrp_las_las_09_02_off_1;
BattleUnitSetup btlparty_las_las_19_01_off_1[1];
BattleGroupSetup btlgrp_las_las_19_01_off_1;
BattleUnitSetup btlparty_las_las_19_02_off_1[2];
BattleGroupSetup btlgrp_las_las_19_02_off_1;
BattleUnitSetup btlparty_las_las_19_03_off_1[3];
BattleGroupSetup btlgrp_las_las_19_03_off_1;
BattleUnitSetup btlparty_las_las_20_01_off_1[1];
BattleGroupSetup btlgrp_las_las_20_01_off_1;
BattleUnitSetup btlparty_las_las_21_01_off_1[2];
BattleGroupSetup btlgrp_las_las_21_01_off_1;
BattleUnitSetup btlparty_las_las_21_02_off_1[2];
BattleGroupSetup btlgrp_las_las_21_02_off_1;
BattleUnitSetup btlparty_las_las_22_01_off_1[2];
BattleGroupSetup btlgrp_las_las_22_01_off_1;
BattleUnitSetup btlparty_las_las_22_02_off_1[4];
BattleGroupSetup btlgrp_las_las_22_02_off_1;
BattleUnitSetup btlparty_las_las_22_03_off_1[2];
BattleGroupSetup btlgrp_las_las_22_03_off_1;
BattleUnitSetup btlparty_las_las_24_01_off_1[2];
BattleGroupSetup btlgrp_las_las_24_01_off_1;
BattleUnitSetup btlparty_las_las_25_01_off_1[1];
BattleGroupSetup btlgrp_las_las_25_01_off_1;
BattleUnitSetup btlparty_las_las_25_02_off_1[3];
BattleGroupSetup btlgrp_las_las_25_02_off_1;
BattleUnitSetup btlparty_las_las_25_03_off_1[4];
BattleGroupSetup btlgrp_las_las_25_03_off_1;
BattleUnitSetup btlparty_las_las_25_04_off_1[2];
BattleGroupSetup btlgrp_las_las_25_04_off_1;
BattleUnitSetup btlparty_las_las_25_05_off_1[3];
BattleGroupSetup btlgrp_las_las_25_05_off_1;
BattleUnitSetup btlparty_las_las_25_06_off_1[3];
BattleGroupSetup btlgrp_las_las_25_06_off_1;
NpcBattleSelect btlgrpsel_las_btlno_las_test1_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_test2_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_test3_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_test4_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_test5_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_test6_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_test7_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_05_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_09_rampell_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_17_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_17_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_17_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_17_04_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_28_koopa_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_bunbaba_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_28_batten_leader_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_29_black_peach_1_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_29_black_peach_2_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_01_01_off[3];
NpcBattleSelect btlgrpsel_las_btlno_las_01_02_off[3];
NpcBattleSelect btlgrpsel_las_btlno_las_01_03_off[3];
NpcBattleSelect btlgrpsel_las_btlno_las_02_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_02_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_02_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_02_04_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_02_05_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_02_06_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_02_99_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_03_01_off[3];
NpcBattleSelect btlgrpsel_las_btlno_las_03_02_off[3];
NpcBattleSelect btlgrpsel_las_btlno_las_03_03_off[3];
NpcBattleSelect btlgrpsel_las_btlno_las_04_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_04_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_05_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_06_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_06_99_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_07_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_07_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_07_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_09_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_09_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_19_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_19_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_19_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_20_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_21_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_21_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_22_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_22_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_22_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_24_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_25_01_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_25_02_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_25_03_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_25_04_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_25_05_off[2];
NpcBattleSelect btlgrpsel_las_btlno_las_25_06_off[2];
NpcSetupBattleInfo btlsetup_las_tbl[61];
NpcTerritoryInfo btl_las_setup_no_tbl[61];
EvtWord bgset_las_gear_event[38];
EvtWord stg_08_1_init_sub_event[97];
EvtWord stg_08_1_init_event[4];
EvtWord stg_08_6_init_sub_event[46];
EvtWord stg_08_6_init_event[4];

USER_FUNC(hi_fire_end) {
    hi_fire_end_flag[0] = 1;
    return EVT_RETURN_DONE;
}

BattleStageObjectData las_btlobject_data[] = {
    {"A1", 2, 0, {93.0f, 0.0f, -64.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C_kabe", 2, 3, {0}, 0, 60, {0}},
    {"gear_01a", 2, 3, {0}, 0, 60, {0}},
    {"gear_01b", 2, 3, {0}, 0, 60, {0}},
    {"gear_02a", 2, 3, {0}, 0, 60, {0}},
    {"gear_02b", 2, 3, {0}, 0, 60, {0}},
    {"gear_03a", 2, 3, {0}, 0, 60, {0}},
    {"gear_03b", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"C", 2, 3, {0.0f, 0.0f, -99.0f}, 0, 60, {0}},
    {"C", 2, 3, {0.0f, 0.0f, -99.0f}, 0, 60, {0}},
    {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"daiza_01", 2, 0, {158.0f, 0.0f, -28.0f}, 0, 60, {0}},
    {"daiza_02", 2, 0, {95.0f, 0.0f, -50.0f}, 10, 50, {0}},
    {"daiza_03", 2, 0, {-158.0f, 0.0f, -28.0f}, 20, 40, {0}},
    {"daiza_04", 2, 0, {-95.0f, 0.0f, -50.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"rou1", 2, 0, {58.0f, 0.0f, -81.0f}, 0, 60, {0}},
    {"rou2", 2, 0, {-109.0f, 0.0f, -69.0f}, 0, 60, {0}},
    {"rou3", 2, 0, {-58.0f, 0.0f, -81.0f}, 0, 60, {0}},
    {"rou4", 2, 0, {179.0f, 0.0f, -54.0f}, 0, 60, {0}},
    {"rou5", 2, 0, {109.0f, 0.0f, -69.0f}, 0, 60, {0}},
    {"rou6", 2, 0, {-179.0f, 0.0f, -54.0f}, 0, 60, {0}},
    {"B", 2, 2, {91.0f, 0.0f, -57.0f}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, 80.0f}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData las_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_5", // mSpecificStageDataDir
        9, // mNumObjects
        las_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_las_gear_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_0", // mSpecificStageDataDir
        2, // mNumObjects
        &las_btlobject_data[9], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_0", // mSpecificStageDataDir
        4, // mNumObjects
        &las_btlobject_data[11], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_2", // mSpecificStageDataDir
        1, // mNumObjects
        &las_btlobject_data[15], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_2", // mSpecificStageDataDir
        3, // mNumObjects
        &las_btlobject_data[16], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_4", // mSpecificStageDataDir
        6, // mNumObjects
        &las_btlobject_data[19], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_4", // mSpecificStageDataDir
        1, // mNumObjects
        &las_btlobject_data[25], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_6", // mSpecificStageDataDir
        8, // mNumObjects
        &las_btlobject_data[26], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100070, 0x10000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        stg_08_6_init_event, // initEvt
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // destroyBgA1Evt..reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_1", // mSpecificStageDataDir
        2, // mNumObjects
        &las_btlobject_data[34], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        stg_08_1_init_event, // initEvt
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // destroyBgA1Evt..reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_08_3", // mSpecificStageDataDir
        2, // mNumObjects
        &las_btlobject_data[36], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_las_las_05_01[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_black_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_05_01 = {
    5, // numEnemies
    btlparty_las_las_05_01, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_17_01_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_17_01_off_1 = {
    1, // numEnemies
    btlparty_las_las_17_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_17_02_off_1[] = {
    {
        &unit_red_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_17_02_off_1 = {
    1, // numEnemies
    btlparty_las_las_17_02_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_17_03_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_17_03_off_1 = {
    1, // numEnemies
    btlparty_las_las_17_03_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_17_04_off_1[] = {
    {
        &unit_black_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_NONE, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_17_04_off_1 = {
    1, // numEnemies
    btlparty_las_las_17_04_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_09_rampell[] = {
    {
        &unit_boss_rampell_las, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {55.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_09_rampell = {
    1, // numEnemies
    btlparty_las_las_09_rampell, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_bunbaba[] = {
    {
        &unit_boss_bunbaba, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {365.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_bunbaba = {
    1, // numEnemies
    btlparty_las_las_bunbaba, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_28_batten_leader[] = {
    {
        &unit_boss_batten_leader, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_28_batten_leader = {
    1, // numEnemies
    btlparty_las_las_28_batten_leader, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_28_koopa[] = {
    {
        &unit_boss_koopa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_28_koopa = {
    1, // numEnemies
    btlparty_las_las_28_koopa, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_29_black_peach_1[] = {
    {
        &unit_boss_black_peach, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_29_black_peach_1 = {
    1, // numEnemies
    btlparty_las_las_29_black_peach_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_29_black_peach_2[] = {
    {
        &unit_boss_black_peach, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_29_black_peach_2 = {
    1, // numEnemies
    btlparty_las_las_29_black_peach_2, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_01_01_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_01_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_01_02_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_01_02_off_1 = {
    3, // numEnemies
    btlparty_las_las_01_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_01_03_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_01_03_off_1 = {
    4, // numEnemies
    btlparty_las_las_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_01_01_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_01_01_off_2 = {
    2, // numEnemies
    btlparty_las_las_01_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_01_02_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_01_02_off_2 = {
    3, // numEnemies
    btlparty_las_las_01_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_01_03_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_01_03_off_2 = {
    4, // numEnemies
    btlparty_las_las_01_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_01_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_02_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_02_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_02_off_1 = {
    3, // numEnemies
    btlparty_las_las_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_03_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_03_off_1 = {
    3, // numEnemies
    btlparty_las_las_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_04_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_04_off_1 = {
    4, // numEnemies
    btlparty_las_las_02_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_05_off_1[] = {
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_05_off_1 = {
    2, // numEnemies
    btlparty_las_las_02_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_06_off_1[] = {
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_06_off_1 = {
    2, // numEnemies
    btlparty_las_las_02_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_02_99_off_1[] = {
    {
        &unit_super_killer, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer, // itemDropTable
    },
    {
        &unit_super_killer, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_02_99_off_1 = {
    2, // numEnemies
    btlparty_las_las_02_99_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_03_01_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_03_01_off_1 = {
    1, // numEnemies
    btlparty_las_las_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_03_02_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_03_02_off_1 = {
    1, // numEnemies
    btlparty_las_las_03_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_03_03_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_03_03_off_1 = {
    1, // numEnemies
    btlparty_las_las_03_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_03_01_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_03_01_off_2 = {
    1, // numEnemies
    btlparty_las_las_03_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_03_02_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_03_02_off_2 = {
    1, // numEnemies
    btlparty_las_las_03_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_03_03_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_03_03_off_2 = {
    1, // numEnemies
    btlparty_las_las_03_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_04_01_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_04_01_off_1 = {
    1, // numEnemies
    btlparty_las_las_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_01_off_1[] = {
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_06_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_02_off_1[] = {
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_02_off_1 = {
    2, // numEnemies
    btlparty_las_las_06_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_03_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_03_off_1 = {
    2, // numEnemies
    btlparty_las_las_06_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_04_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_04_off_1 = {
    4, // numEnemies
    btlparty_las_las_06_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_05_off_1[] = {
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_05_off_1 = {
    2, // numEnemies
    btlparty_las_las_06_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_06_off_1[] = {
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
    {
        &unit_super_killer_cannon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer_cannon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_06_off_1 = {
    2, // numEnemies
    btlparty_las_las_06_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_06_99_off_1[] = {
    {
        &unit_super_killer, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer, // itemDropTable
    },
    {
        &unit_super_killer, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_killer, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_06_99_off_1 = {
    2, // numEnemies
    btlparty_las_las_06_99_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_07_01_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_07_01_off_1 = {
    3, // numEnemies
    btlparty_las_las_07_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_07_02_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_07_02_off_1 = {
    3, // numEnemies
    btlparty_las_las_07_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_07_03_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_07_03_off_1 = {
    3, // numEnemies
    btlparty_las_las_07_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_09_01_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_09_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_09_02_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_09_02_off_1 = {
    3, // numEnemies
    btlparty_las_las_09_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_19_01_off_1[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_19_01_off_1 = {
    1, // numEnemies
    btlparty_las_las_19_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_19_02_off_1[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_19_02_off_1 = {
    2, // numEnemies
    btlparty_las_las_19_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_19_03_off_1[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_19_03_off_1 = {
    3, // numEnemies
    btlparty_las_las_19_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_20_01_off_1[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_20_01_off_1 = {
    1, // numEnemies
    btlparty_las_las_20_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_21_01_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_21_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_21_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_21_02_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_21_02_off_1 = {
    2, // numEnemies
    btlparty_las_las_21_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_22_01_off_1[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_22_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_22_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_22_02_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_22_02_off_1 = {
    4, // numEnemies
    btlparty_las_las_22_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_22_03_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_22_03_off_1 = {
    2, // numEnemies
    btlparty_las_las_22_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_24_01_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_24_01_off_1 = {
    2, // numEnemies
    btlparty_las_las_24_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_25_01_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_25_01_off_1 = {
    1, // numEnemies
    btlparty_las_las_25_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_25_02_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_25_02_off_1 = {
    3, // numEnemies
    btlparty_las_las_25_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_25_03_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_25_03_off_1 = {
    4, // numEnemies
    btlparty_las_las_25_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_25_04_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_25_04_off_1 = {
    2, // numEnemies
    btlparty_las_las_25_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_25_05_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_25_05_off_1 = {
    3, // numEnemies
    btlparty_las_las_25_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_las_las_25_06_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_las_las_25_06_off_1 = {
    3, // numEnemies
    btlparty_las_las_25_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_las_btlno_las_test1_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_test2_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_test3_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_test4_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[9]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_test5_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_test6_off[] = {
    {50, &btlgrp_las_las_05_01, las_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_test7_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[7]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_05_01_off[] = {
    {50, &btlgrp_las_las_05_01, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_09_rampell_off[] = {
    {50, &btlgrp_las_las_09_rampell, &las_btlstage_data[9]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_17_01_off[] = {
    {50, &btlgrp_las_las_17_01_off_1, &las_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_17_02_off[] = {
    {50, &btlgrp_las_las_17_02_off_1, &las_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_17_03_off[] = {
    {50, &btlgrp_las_las_17_03_off_1, &las_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_17_04_off[] = {
    {50, &btlgrp_las_las_17_04_off_1, &las_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_28_koopa_off[] = {
    {50, &btlgrp_las_las_28_koopa, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_bunbaba_off[] = {
    {50, &btlgrp_las_las_bunbaba, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_28_batten_leader_off[] = {
    {50, &btlgrp_las_las_28_batten_leader, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_29_black_peach_1_off[] = {
    {50, &btlgrp_las_las_29_black_peach_1, &las_btlstage_data[7]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_29_black_peach_2_off[] = {
    {50, &btlgrp_las_las_29_black_peach_2, &las_btlstage_data[7]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_01_01_off[] = {
    {20, &btlgrp_las_las_01_01_off_1, &las_btlstage_data[1]},
    {0, &btlgrp_las_las_01_01_off_2, &las_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_01_02_off[] = {
    {20, &btlgrp_las_las_01_02_off_1, &las_btlstage_data[1]},
    {0, &btlgrp_las_las_01_02_off_2, &las_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_01_03_off[] = {
    {20, &btlgrp_las_las_01_03_off_1, &las_btlstage_data[1]},
    {0, &btlgrp_las_las_01_03_off_2, &las_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_01_off[] = {
    {20, &btlgrp_las_las_02_01_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_02_off[] = {
    {20, &btlgrp_las_las_02_02_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_03_off[] = {
    {20, &btlgrp_las_las_02_03_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_04_off[] = {
    {20, &btlgrp_las_las_02_04_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_05_off[] = {
    {20, &btlgrp_las_las_02_05_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_06_off[] = {
    {20, &btlgrp_las_las_02_06_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_02_99_off[] = {
    {20, &btlgrp_las_las_02_99_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_03_01_off[] = {
    {20, &btlgrp_las_las_03_01_off_1, &las_btlstage_data[3]},
    {0, &btlgrp_las_las_03_01_off_2, &las_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_03_02_off[] = {
    {20, &btlgrp_las_las_03_02_off_1, &las_btlstage_data[3]},
    {0, &btlgrp_las_las_03_02_off_2, &las_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_03_03_off[] = {
    {20, &btlgrp_las_las_03_03_off_1, &las_btlstage_data[3]},
    {0, &btlgrp_las_las_03_03_off_2, &las_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_04_01_off[] = {
    {20, &btlgrp_las_las_04_01_off_1, &las_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_01_off[] = {
    {20, &btlgrp_las_las_06_01_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_02_off[] = {
    {20, &btlgrp_las_las_06_02_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_03_off[] = {
    {20, &btlgrp_las_las_06_03_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_04_off[] = {
    {20, &btlgrp_las_las_06_04_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_05_off[] = {
    {20, &btlgrp_las_las_06_05_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_06_off[] = {
    {20, &btlgrp_las_las_06_06_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_06_99_off[] = {
    {20, &btlgrp_las_las_06_99_off_1, &las_btlstage_data[8]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_07_01_off[] = {
    {20, &btlgrp_las_las_07_01_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_07_02_off[] = {
    {20, &btlgrp_las_las_07_02_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_07_03_off[] = {
    {20, &btlgrp_las_las_07_03_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_09_01_off[] = {
    {20, &btlgrp_las_las_09_01_off_1, &las_btlstage_data[9]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_09_02_off[] = {
    {20, &btlgrp_las_las_09_02_off_1, &las_btlstage_data[9]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_19_01_off[] = {
    {20, &btlgrp_las_las_19_01_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_19_02_off[] = {
    {20, &btlgrp_las_las_19_02_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_19_03_off[] = {
    {20, &btlgrp_las_las_19_03_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_20_01_off[] = {
    {20, &btlgrp_las_las_20_01_off_1, las_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_21_01_off[] = {
    {20, &btlgrp_las_las_21_01_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_21_02_off[] = {
    {20, &btlgrp_las_las_21_02_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_22_01_off[] = {
    {20, &btlgrp_las_las_22_01_off_1, las_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_22_02_off[] = {
    {20, &btlgrp_las_las_22_02_off_1, las_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_22_03_off[] = {
    {20, &btlgrp_las_las_22_03_off_1, las_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_24_01_off[] = {
    {20, &btlgrp_las_las_24_01_off_1, las_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_25_01_off[] = {
    {20, &btlgrp_las_las_25_01_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_25_02_off[] = {
    {20, &btlgrp_las_las_25_02_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_25_03_off[] = {
    {20, &btlgrp_las_las_25_03_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_25_04_off[] = {
    {20, &btlgrp_las_las_25_04_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_25_05_off[] = {
    {20, &btlgrp_las_las_25_05_off_1, &las_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_las_btlno_las_25_06_off[] = {
    {20, &btlgrp_las_las_25_06_off_1, &las_btlstage_data[3]},
    {0},
};

NpcSetupBattleInfo btlsetup_las_tbl[] = {
    {
        /*"マップstg_08_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_08_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_08_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_08_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test4_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_08_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test5_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_08_5"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x35", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test6_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_08_6"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x38\x5F\x36", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_test7_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"黒カロン"*/"\x8D\x95\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_05_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"変身ランペル+魔女"*/"\x95\xCF\x90\x67\x83\x89\x83\x93\x83\x79\x83\x8B\x2B\x96\x82\x8F\x97", // name
        "las_09", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_09_rampell_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_3WOMEN1", // bgmName
    },
    {
        /*"las17クリボー"*/"\x6C\x61\x73\x31\x37\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_17_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las17トゲクリボー"*/"\x6C\x61\x73\x31\x37\x83\x67\x83\x51\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_17_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las17ノコノコ"*/"\x6C\x61\x73\x31\x37\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_17_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las17シンノスケ"*/"\x6C\x61\x73\x31\x37\x83\x56\x83\x93\x83\x6D\x83\x58\x83\x50", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_17_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"クッパ＆カメックオババ"*/"\x83\x4E\x83\x62\x83\x70\x81\x95\x83\x4A\x83\x81\x83\x62\x83\x4E\x83\x49\x83\x6F\x83\x6F", // name
        "las_28", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_28_koopa_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG8_KOOPA", // bgmName
    },
    {
        /*"ブンババ"*/"\x83\x75\x83\x93\x83\x6F\x83\x6F", // name
        "las_26", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_bunbaba_off, // select
        0x20100010, // battleFlags
        6, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG1_GONBABA1", // bgmName
    },
    {
        /*"バッテンリーダー"*/"\x83\x6F\x83\x62\x83\x65\x83\x93\x83\x8A\x81\x5B\x83\x5F\x81\x5B", // name
        "las_28", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_28_batten_leader_off, // select
        0x20000010, // battleFlags
        5, // field_1C
        {{0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG8_3RDHEAD1", // bgmName
    },
    {
        /*"ブラックピーチ1回目"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60\x31\x89\xF1\x96\xDA", // name
        "las_29", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_29_black_peach_1_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG8_WITCH", // bgmName
    },
    {
        /*"ブラックピーチ2回目"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60\x32\x89\xF1\x96\xDA", // name
        "las_29", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_29_black_peach_2_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG8_LAST_BOSS2", // bgmName
    },
    {
        /*"las01バサバサチュルル"*/"\x6C\x61\x73\x30\x31\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_01_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las01バサバサチュルル"*/"\x6C\x61\x73\x30\x31\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_01_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las01バサバサチュルル"*/"\x6C\x61\x73\x30\x31\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_01_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02カロン"*/"\x6C\x61\x73\x30\x32\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02バサバサチュルル"*/"\x6C\x61\x73\x30\x32\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02カロン"*/"\x6C\x61\x73\x30\x32\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02カロン"*/"\x6C\x61\x73\x30\x32\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02スーパーキラー大砲"*/"\x6C\x61\x73\x30\x32\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02スーパーキラー大砲"*/"\x6C\x61\x73\x30\x32\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_06_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las02スーパーキラー"*/"\x6C\x61\x73\x30\x32\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_02_99_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las03バサバサチュルル"*/"\x6C\x61\x73\x30\x33\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las03バサバサチュルル"*/"\x6C\x61\x73\x30\x33\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_03_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las03バサバサチュルル"*/"\x6C\x61\x73\x30\x33\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_03_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las04ファントム"*/"\x6C\x61\x73\x30\x34\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_04_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06スーパーキラー大砲"*/"\x6C\x61\x73\x30\x36\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06スーパーキラー大砲"*/"\x6C\x61\x73\x30\x36\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06ファントム"*/"\x6C\x61\x73\x30\x36\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06ファントム"*/"\x6C\x61\x73\x30\x36\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06スーパーキラー大砲"*/"\x6C\x61\x73\x30\x36\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06スーパーキラー大砲"*/"\x6C\x61\x73\x30\x36\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B\x91\xE5\x96\x43", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_06_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las06スーパーキラー"*/"\x6C\x61\x73\x30\x36\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x89\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_06_99_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las07ファントム"*/"\x6C\x61\x73\x30\x37\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_07_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las07カロン"*/"\x6C\x61\x73\x30\x37\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_07_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las07カロン"*/"\x6C\x61\x73\x30\x37\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_07_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las09ワンワン"*/"\x6C\x61\x73\x30\x39\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_09_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las09ワンワン"*/"\x6C\x61\x73\x30\x39\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_09_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las19スーパーマホーン"*/"\x6C\x61\x73\x31\x39\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_19_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las19スーパーマホーン"*/"\x6C\x61\x73\x31\x39\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_19_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las19スーパーマホーン"*/"\x6C\x61\x73\x31\x39\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_19_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las20スーパーマホーン"*/"\x6C\x61\x73\x32\x30\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_20_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las21ワンワン"*/"\x6C\x61\x73\x32\x31\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_21_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las21ワンワン"*/"\x6C\x61\x73\x32\x31\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_21_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las22スーパーマホーン"*/"\x6C\x61\x73\x32\x32\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_22_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las22ファントム"*/"\x6C\x61\x73\x32\x32\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_22_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las22ワンワン"*/"\x6C\x61\x73\x32\x32\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_22_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las24ワンワン"*/"\x6C\x61\x73\x32\x34\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_24_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las25ファントム"*/"\x6C\x61\x73\x32\x35\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_25_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las25ファントム"*/"\x6C\x61\x73\x32\x35\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_25_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las25ファントム"*/"\x6C\x61\x73\x32\x35\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_25_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las25ファントム"*/"\x6C\x61\x73\x32\x35\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_25_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las25ファントム"*/"\x6C\x61\x73\x32\x35\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_25_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"las25ファントム"*/"\x6C\x61\x73\x32\x35\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_las_btlno_las_25_06_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_las_setup_no_tbl[] = {
    {"BTLNO_LAS_TEST1", 0},
    {"BTLNO_LAS_TEST2", 1},
    {"BTLNO_LAS_TEST3", 2},
    {"BTLNO_LAS_TEST4", 3},
    {"BTLNO_LAS_TEST5", 4},
    {"BTLNO_LAS_TEST6", 5},
    {"BTLNO_LAS_TEST7", 6},
    {"BTLNO_LAS_05_01", 7},
    {"BTLNO_LAS_09_RAMPELL", 8},
    {"BTLNO_LAS_17_01", 9},
    {"BTLNO_LAS_17_02", 10},
    {"BTLNO_LAS_17_03", 11},
    {"BTLNO_LAS_17_04", 12},
    {"BTLNO_LAS_28_KOOPA", 13},
    {"BTLNO_LAS_BUNBABA", 14},
    {"BTLNO_LAS_28_BATTEN_LEADER", 15},
    {"BTLNO_LAS_29_BLACK_PEACH_1", 16},
    {"BTLNO_LAS_29_BLACK_PEACH_2", 17},
    {"BTLNO_LAS_01_01", 18},
    {"BTLNO_LAS_01_02", 19},
    {"BTLNO_LAS_01_03", 20},
    {"BTLNO_LAS_02_01", 21},
    {"BTLNO_LAS_02_02", 22},
    {"BTLNO_LAS_02_03", 23},
    {"BTLNO_LAS_02_04", 24},
    {"BTLNO_LAS_02_05", 25},
    {"BTLNO_LAS_02_06", 26},
    {"BTLNO_LAS_02_99", 27},
    {"BTLNO_LAS_03_01", 28},
    {"BTLNO_LAS_03_02", 29},
    {"BTLNO_LAS_03_03", 30},
    {"BTLNO_LAS_04_01", 31},
    {"BTLNO_LAS_06_01", 32},
    {"BTLNO_LAS_06_02", 33},
    {"BTLNO_LAS_06_03", 34},
    {"BTLNO_LAS_06_04", 35},
    {"BTLNO_LAS_06_05", 36},
    {"BTLNO_LAS_06_06", 37},
    {"BTLNO_LAS_06_99", 38},
    {"BTLNO_LAS_07_01", 39},
    {"BTLNO_LAS_07_02", 40},
    {"BTLNO_LAS_07_03", 41},
    {"BTLNO_LAS_09_01", 42},
    {"BTLNO_LAS_09_02", 43},
    {"BTLNO_LAS_19_01", 44},
    {"BTLNO_LAS_19_02", 45},
    {"BTLNO_LAS_19_03", 46},
    {"BTLNO_LAS_20_01", 47},
    {"BTLNO_LAS_21_01", 48},
    {"BTLNO_LAS_21_02", 49},
    {"BTLNO_LAS_22_01", 50},
    {"BTLNO_LAS_22_02", 51},
    {"BTLNO_LAS_22_03", 52},
    {"BTLNO_LAS_24_01", 53},
    {"BTLNO_LAS_25_01", 54},
    {"BTLNO_LAS_25_02", 55},
    {"BTLNO_LAS_25_03", 56},
    {"BTLNO_LAS_25_04", 57},
    {"BTLNO_LAS_25_05", 58},
    {"BTLNO_LAS_25_06", 59},
    {0},
};

EVT_BEGIN(bgset_las_gear_event)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    LABEL(0)
    CALL(_gear_rotate, STRING("gear_01a"), STRING("gear_01b"), LW(0), 400, -1)
    CALL(_gear_rotate, STRING("gear_02a"), STRING("gear_02b"), LW(1), 600, 1)
    CALL(_gear_rotate, STRING("gear_03a"), STRING("gear_03b"), LW(2), 800, 1)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(stg_08_1_init_sub_event)
    CALL(evt_mapobj_get_position, STRING("hi1"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("_hi1"), STRING("fire"), 3, LW(0), LW(1), LW(2), FLOAT(1.0f), 1000, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_get_position, STRING("hi2"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("_hi2"), STRING("fire"), 3, LW(0), LW(1), LW(2), FLOAT(1.0f), 1000, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_get_position, STRING("hi3"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("_hi3"), STRING("fire"), 3, LW(0), LW(1), LW(2), FLOAT(1.0f), 1000, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi1"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi2"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi3"), 1)
    CALL(check_battle_end)
    CALL(evt_eff_softdelete, STRING("_hi1"))
    CALL(evt_eff_softdelete, STRING("_hi2"))
    CALL(evt_eff_softdelete, STRING("_hi3"))
    RETURN
EVT_END()

EVT_BEGIN(stg_08_1_init_event)
    SPAWN_EVENT(PTR(stg_08_1_init_sub_event))
    RETURN
EVT_END()

EVT_BEGIN(stg_08_6_init_sub_event)
    BEGIN_SIBLING_EVENT
        CALL(hi_fire_main)
    END_SIBLING_EVENT
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi1"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi2"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi3"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi4"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi5"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("hi6"), 1)
    CALL(check_battle_end)
    CALL(hi_fire_end)
    RETURN
EVT_END()

USER_FUNC(hi_fire_main) {
    const char* names[6] = {"hi1", "hi2", "hi3", "hi4", "hi5", "hi6"};
    Vec pos;
    s32 i;

    if (isFirstCall) {
        hi_fire_end_flag[0] = 0;
        for (i = 0; i < 6; i++) {
            hi_fire_eff[i] = effFireEntry(8, 0.0f, -1000.0f, 0.0f, 1.0f, 1000);
        }
    }
    if (hi_fire_end_flag[0]) {
        for (i = 0; i < 6; i++) {
            effSoftDelete(hi_fire_eff[i]);
        }
        return EVT_RETURN_DONE;
    }
    for (i = 0; i < 6; i++) {
        mapObjGetPos(names[i], &pos);
        if (pos.y > 0.0f) {
            f32* wk = hi_fire_eff[i]->userdata;

            wk[1] = pos.x;
            wk[2] = pos.y - 15.0f;
            wk[3] = pos.z;
        }
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(check_battle_end) {
    return battleSeqEndCheck() ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(_gear_rotate) {
    EvtWord* args = event->args;
    const char* name1 = (const char*)evtGetValue(event, args[0]);
    const char* name2 = (const char*)evtGetValue(event, args[1]);
    s32 frameVar = args[2];
    s32 frame = evtGetValue(event, frameVar);
    s32 total = evtGetValue(event, args[3]);
    s32 step = evtGetValue(event, args[4]);
    BattleStageObject* obj1 = BattleSearchObjectPtr(name1);
    BattleStageObject* obj2 = BattleSearchObjectPtr(name2);

    obj2->mRotation.z = obj1->mRotation.z = 360.0f * frame / total;
    frame += step;
    if (frame >= total) {
        frame = 0;
    }
    if (frame < 0) {
        frame = total - 1;
    }
    evtSetValue(event, frameVar, frame);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(stg_08_6_init_event)
    SPAWN_EVENT(PTR(stg_08_6_init_sub_event))
    RETURN
EVT_END()

