#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "battle/battle_stage_object.h"
#include "battle/battle.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"

USER_FUNC(_set_destroy_flag);
USER_FUNC(_set_offset);
extern BattleUnitKind unit_boss_moamoa;

BattleStageObjectData rsh_btlobject_data[7];
BattleStageData rsh_btlstage_data[1];
BattleUnitSetup btlparty_rsh_rsh_06_01_off_1[1];
BattleGroupSetup btlgrp_rsh_rsh_06_01_off_1;
NpcBattleSelect btlgrpsel_rsh_btlno_rsh_test1_off[2];
NpcBattleSelect btlgrpsel_rsh_btlno_rsh_06_01_off[2];
NpcSetupBattleInfo btlsetup_rsh_tbl[3];
NpcTerritoryInfo btl_rsh_setup_no_tbl[3];
EvtWord bgset_rsh_bg_event[30];

BattleStageObjectData rsh_btlobject_data[] = {
    {"r_mae", 2, 1, {170.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"r_oku", 2, 1, {170.0f, -1000.0f, -50.0f}, 10, 50, {0}},
    {"l_mae", 2, 1, {-170.0f, 0.0f, -50.0f}, 20, 40, {0}},
    {"l_oku", 2, 1, {-170.0f, -1000.0f, -50.0f}, 30, 30, {0}},
    {"iwayama_1", 2, 2, {0}, 0, 60, {0}},
    {"iwayama_2", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData rsh_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_06_0", // mSpecificStageDataDir
        7, // mNumObjects
        rsh_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgA2Evt
        bgset_rsh_bg_event, // bgBEvt
        NULL, 0, 0, 0, 0, // bgCEvt..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_rsh_rsh_06_01_off_1[] = {
    {
        &unit_boss_moamoa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, -20.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_moamoa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_rsh_rsh_06_01_off_1 = {
    1, // numEnemies
    btlparty_rsh_rsh_06_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // reserved_1C
};

NpcBattleSelect btlgrpsel_rsh_btlno_rsh_test1_off[] = {
    {50, &btlgrp_rsh_rsh_06_01_off_1, rsh_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_rsh_btlno_rsh_06_01_off[] = {
    {50, &btlgrp_rsh_rsh_06_01_off_1, rsh_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_rsh_tbl[] = {
    {
        /*"マップstg_06_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x36\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_rsh_btlno_rsh_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"rsh_06 もあもあ"*/"\x72\x73\x68\x5F\x30\x36\x20\x82\xE0\x82\xA0\x82\xE0\x82\xA0", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_rsh_btlno_rsh_06_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        6, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG6_GREAT_MOAMOA1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_rsh_setup_no_tbl[] = {
    {"BTLNO_RSH_TEST1", 0},
    {"BTLNO_RSH_06_01", 1},
    {0},
};

EVT_BEGIN(bgset_rsh_bg_event)
    CALL(_set_destroy_flag)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 200)
    LABEL(0)
    CALL(_set_offset, STRING("iwayama_1"), LW(0), 400, -300, 800)
    CALL(_set_offset, STRING("iwayama_2"), LW(1), 400, -400, 800)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

USER_FUNC(_set_offset) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 outVar = args[1];
    s32 step = evtGetValue(event, outVar);
    s32 total = evtGetValue(event, args[2]);
    s32 base = evtGetValue(event, args[3]);
    s32 amp = evtGetValue(event, args[4]);
    BattleStageObject* obj = BattleSearchObjectPtr(name);

    obj->positionOffset.x = base + amp * step / total;
    step--;
    if (step < 0) {
        step = total;
    }
    evtSetValue(event, outVar, step);
    return EVT_RETURN_DONE;
}
USER_FUNC(_set_destroy_flag) {
    _battleWorkPointer->stageObjectWork.flags |= 8;
    return EVT_RETURN_DONE;
}
