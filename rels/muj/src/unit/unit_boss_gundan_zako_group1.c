#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"
#include "battle/battle_status_effect.h"
#include "rels/muj/include/muj.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_stage_object.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"

typedef struct ZakoGun {
    s8 field_0x0;
    s8 state;
    s32 timer;
    s32 timerMax;
    Vec pos;
    Vec home;
    s32 poseBase;
    s32 poseNum;
    s32 poseDiv;
    s32 poseCount;
    Vec start;
    Vec target;
    Vec vel;
    Vec ofs;
    Vec rot;
} ZakoGun;
typedef struct ZakoGunWork { s32 flag; s32 targetId;
#ifdef VERSION_G8ME01
    u8 us_pad08[0xC]; // US: 12 more bytes here
#endif
    s32 state; s32 timer; Vec pos; u8 field_0x1c[0x34 - 0x1c]; f32 radius; f32 angle; f32 speed; u8 field_0x40[0x48 - 0x40]; Vec start; Vec end; s32 timer2; s32 steps; s32 count; s32 sfx; ZakoGun* zako; } ZakoGunWork;
extern ZakoGun* zakogun;
extern ZakoGunWork* work;
void zakogun_ext_main1(void);
void zakogun_ext_disp(CameraId camera, void* param);

USER_FUNC(zakogun_attack1);
USER_FUNC(zakogun_damage_pose);
USER_FUNC(zakogun_damage_pose_cancel);
USER_FUNC(zakogun_end_sukima);
USER_FUNC(zakogun_set_target_id);
USER_FUNC(zakogun_start_sukima);
USER_FUNC(zakogun_escape1);
USER_FUNC(zakogun_dead1);
void zakogun_ext_init123_common(ExtdrvPose* pose, s32 i);
USER_FUNC(zakogun_ext_init1);
USER_FUNC(zakogun_get_kanbu_id_evtcmd);
void zakogun_ext_main(void);
void zakogun_ext_init(void);
void zakogun_set_pose(ZakoGun* zako, s32 base, s32 num, s32 div);

BattleUnitKind unit_boss_gundan_zako_group1;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[4];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_gundan_zako_group1_attack;
static EvtWord init_event[44];
static EvtWord damage_event[134];
EvtWord damage_event_kaiten_hammer[30];
static EvtWord dead_event[138];
static EvtWord attack_event[85];
static EvtWord hit_check_event[95];
static EvtWord wait_event[6];
ExtTexInfo zakogun_ext_entry_data[49];
EvtWord zakogun_entry[19];

USER_FUNC(zakogun_set_target_id) {
    work->targetId = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

void zakogun_set_pose(ZakoGun* zako, s32 base, s32 num, s32 div) {
    zako->poseBase = base;
    zako->poseNum = num;
    zako->poseDiv = div;
    zako->poseCount = 0;
}

USER_FUNC(zakogun_damage_pose) {
    s32 i;
    ZakoGun* zako;

    if (isFirstCall) {
        for (i = 0; i < 500; i++) {
            zako = &zakogun[i];
            if (zako != NULL) {
                switch (work->flag) {
                    case 0:
                    case 2:
                        zakogun_set_pose(zako, 0x21, 3, 3);
                        break;
                    case 1:
                        zakogun_set_pose(zako, 0x2d, 3, 2);
                        break;
                }
            }
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(zakogun_damage_pose_cancel) {
    s32 i;
    ZakoGun* zako;

    if (isFirstCall) {
        for (i = 0; i < 500; i++) {
            zako = &zakogun[i];
            if (zako != NULL) {
                switch (work->flag) {
                    case 0:
                        zakogun_set_pose(zako, 0x15, 1, 1);
                        break;
                    case 1:
                        zakogun_set_pose(zako, 0x25, 4, 6);
                        break;
                    case 2:
                        zakogun_set_pose(zako, 0, 20, 2);
                        break;
                }
            }
        }
    }
    return EVT_RETURN_DONE;
}

BattleUnit* zakogun_get_ptr(void) {
    BattleUnit* unit;
    s32 i;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && (unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION1 || unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION2 || unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION3)) {
            return unit;
        }
    }
    return NULL;
}

BattleUnitKind unit_boss_gundan_zako_group1 = {
    100,
    "btl_un_gundan_zako_group1",
    6,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    9999,
    50,
    50,
    0,
    50,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    25.0f,
    32.5f,
    0.0f,
    -4.0f,
    25.0f,
    0.0f,
    50.0f,
    50.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_TEAM_3RD_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x200000,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {28, "Z_1"},
    {31, "Z_1"},
    {39, "Z_1"},
    {69, "Z_1"},
};

static BattleDataEntry data_table[] = {
    {18, damage_event_kaiten_hammer},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako_group1", "c_zako_n", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x3000009, 0, pose_table},
};

BattleWeapon weapon_gundan_zako_group1_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    4, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), 15)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), 15)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamage, -2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SET_VALUE(LW(15), 1)
        ELSE
        SET_VALUE(LW(15), 0)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_INSTANT_KILL, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_SLEEP, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_DIZZY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_CONFUSE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_FREEZE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_STOP, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_POISON, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_BURN, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_DEFENSE_DOWN, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_SLOW, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_ALLERGIC, LW(14))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(zakogun_damage_pose)
        CALL(zakogun_start_sukima, 0)
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(zakogun_damage_pose_cancel)
    CALL(zakogun_end_sukima)
    RETURN
EVT_END()

EVT_BEGIN(damage_event_kaiten_hammer)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamage, -2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(zakogun_damage_pose)
        CALL(zakogun_start_sukima, 1)
        WAIT_FRAMES(80)
        CALL(zakogun_damage_pose_cancel)
        CALL(zakogun_end_sukima)
        ELSE
        WAIT_FRAMES(80)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    CALL(zakogun_dead1)
    WAIT_FRAMES(36)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(48)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(10)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(evt_sub_random, 100, LW(3))
            SUBTRACT(LW(3), 50)
            ADD(LW(0), LW(3))
            CALL(evt_sub_random, 30, LW(3))
            ADD(LW(1), LW(3))
            SET_VALUE(LW(2), 75)
            CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetCoin, -2, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, -2, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, -2, LW(3), LW(1), LW(2))
    CALL(_add_star_point_disp_offset, -2, LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, -2, 0)
    ELSE
        CALL(btlevtcmd_KillUnit, -2, 2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_group1_attack))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(zakogun_set_target_id, LW(3))
    BEGIN_SIBLING_EVENT
        CALL_CHILD(PTR(hit_check_event))
    END_SIBLING_EVENT
    CALL(zakogun_attack1)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(hit_check_event)
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    GOTO(98)
    LABEL(98)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

ExtTexInfo zakogun_ext_entry_data[] = {
    {"c_zako_n", "S_1", 0.0f},
    {"c_zako_n", "S_1", 2.0f},
    {"c_zako_n", "S_1", 4.0f},
    {"c_zako_n", "S_1", 6.0f},
    {"c_zako_n", "S_1", 8.0f},
    {"c_zako_n", "S_1", 10.0f},
    {"c_zako_n", "S_1", 12.0f},
    {"c_zako_n", "S_1", 14.0f},
    {"c_zako_n", "S_1", 16.0f},
    {"c_zako_n", "S_1", 18.0f},
    {"c_zako_n", "S_1", 20.0f},
    {"c_zako_n", "S_1", 22.0f},
    {"c_zako_n", "S_1", 24.0f},
    {"c_zako_n", "S_1", 26.0f},
    {"c_zako_n", "S_1", 28.0f},
    {"c_zako_n", "S_1", 30.0f},
    {"c_zako_n", "S_1", 32.0f},
    {"c_zako_n", "S_1", 34.0f},
    {"c_zako_n", "S_1", 36.0f},
    {"c_zako_n", "S_1", 38.0f},
    {"c_zako_n", "Z_2", 0.0f},
    {"c_zako_n", "Z_3", 0.0f},
    {"c_zako_n", "R_1", 0.0f},
    {"c_zako_n", "R_1", 1.0f},
    {"c_zako_n", "R_1", 2.0f},
    {"c_zako_n", "R_1", 3.0f},
    {"c_zako_n", "R_1", 4.0f},
    {"c_zako_n", "R_1", 5.0f},
    {"c_zako_n", "R_1", 6.0f},
    {"c_zako_n", "R_1", 7.0f},
    {"c_zako_n", "R_1", 8.0f},
    {"c_zako_n", "R_1", 9.0f},
    {"c_zako_n", "A_3", 0.0f},
    {"c_zako_n", "D_1", 0.0f},
    {"c_zako_n", "D_1", 3.0f},
    {"c_zako_n", "D_1", 6.0f},
    {"c_zako_n", "J_1A", 0.0f},
    {"c_zako_n", "S_2", 0.0f},
    {"c_zako_n", "S_2", 10.0f},
    {"c_zako_n", "S_2", 20.0f},
    {"c_zako_n", "S_2", 30.0f},
    {"c_zako_n", "A_5", 0.0f},
    {"c_zako_n", "A_5", 8.0f},
    {"c_zako_n", "A_5", 12.0f},
    {"c_zako_n", "A_2", 0.0f},
    {"c_zako_n", "D_5", 0.0f},
    {"c_zako_n", "D_5", 2.0f},
    {"c_zako_n", "D_5", 4.0f},
    {0},
};

EVT_BEGIN(zakogun_entry)
    CALL(evt_npc_entry, STRING("ext_zakogun"), STRING("c_zako_n"))
    CALL(evt_npc_set_position, STRING("ext_zakogun"), 0, -1000, 0)
    CALL(evt_ext_entry, 500, PTR(zakogun_ext_entry_data), PTR(zakogun_ext_init), PTR(zakogun_ext_main), 0)
    RETURN
EVT_END()

void zakogun_ext_init(void) {
    u8* flags = &_battleWorkPointer->stageObjectWork.flags;
    BattleStageObject* body;
    BattleStageObject* wire;

    work = BattleAlloc(sizeof(ZakoGunWork));
    work->zako = BattleAlloc(500 * sizeof(ZakoGun));
    zakogun = work->zako;
    work->flag = 0;
    body = BattleSearchObjectPtr("if_body");
    wire = BattleSearchObjectPtr("if_wire");
    wire->mPosition = (Vec){0.0f, 250.0f, 0.0f};
    body->mPosition = wire->mPosition;
    *flags |= 0x60;
}

void zakogun_ext_main(void) {
    switch (work->flag) {
        case 0:
            zakogun_ext_main1();
            break;
        case 1:
            zakogun_ext_main2();
            break;
        case 2:
            zakogun_ext_main3();
            break;
    }
    dispEntry(CAMERA_3D, 0.0f, 1, zakogun_ext_disp, NULL);
}

void zakogun_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

USER_FUNC(zakogun_get_kanbu_id_evtcmd) {
    BattleUnit* unit;
    s32 id = -1;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    EvtWord* args = event->args;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_LORD_CRUMP_CHAPTER5) {
            id = i;
        }
    }
    evtSetValue(event, args[0], id);
    return EVT_RETURN_DONE;
}

s32 zakogun_get_kanbu_id(void) {
    s32 i;
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    s32 id = -1;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_LORD_CRUMP_CHAPTER5) {
            id = i;
        }
    }
    return id;
}

USER_FUNC(zakogun_ext_init1) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    work->flag = 0;
    for (i = 0; i < 6; i++, pose++) {
        zakogun_ext_init123_common(pose, i);
    }
    return EVT_RETURN_DONE;
}

void zakogun_ext_init123_common(ExtdrvPose* pose, s32 i) {
    pose->flags = 1;
    PSMTXIdentity(*pose->mtx);
    (*pose->mtx)[0][3] = 0.0f;
    (*pose->mtx)[1][3] = -1000.0f;
    (*pose->mtx)[2][3] = 0.0f;
    pose->dir = 90.0f;
    zakogun[i].field_0x0 = 0;
    zakogun[i].state = 0;
    zakogun[i].timer = 0;
    zakogun[i].pos = (Vec){0.0f, -1000.0f, 0.0f};
    zakogun[i].home = (Vec){0.0f, 0.0f, 0.0f};
    zakogun[i].rot.y = 0.0f;
}

void zakogun_ext_main1(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = zakogun_get_ptr();
    ExtdrvPose* pose = extGetPosePtr();
    ZakoGun* zako;
    s32 i;
    s32 n;
    s32 j;
    f32 x, y, z;
    f32 d;
    Mtx m;

    for (i = 0; i < 6; i++, pose++) {
        zako = &zakogun[i];
        pose->texId = zako->poseBase + zako->poseCount / zako->poseDiv;
        zako->poseCount++;
        if (zako->poseCount >= zako->poseNum * zako->poseDiv) {
            zako->poseCount = 0;
        }
        if (i == 1 && zako->state >= 35 && zako->state < 80) {
            unit->mPosition.x = zako->pos.x;
        }
        switch (zako->state) {
            case 0:
                zako->state = 5;
                zako->timer = 0;
                zako->rot.x = zako->rot.y = zako->rot.z = 0.0f;
                zakogun_set_pose(zako, 0, 20, 2);
                break;
            case 5:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->timer = 0;
                    zako->state = 10;
                }
                break;
            case 10:
                zako->pos.x = zako->start.x = 300.0f + irand(100);
                zako->pos.y = zako->start.y = 0.0f;
                zako->pos.z = zako->start.z = irand(100) - 50.0f;
                zako->state = 12;
                zako->rot.y = 0.0f;
            case 12:
                x = unit->mHomePosition.x;
                y = unit->mHomePosition.y;
                z = unit->mHomePosition.z;
                switch (i) {
                    case 0:
                        zako->target.x = x - 10.0f;
                        zako->target.y = y;
                        zako->target.z = z - 10.0f;
                        break;
                    case 1:
                        zako->target.x = x;
                        zako->target.y = y;
                        zako->target.z = z;
                        break;
                    case 2:
                        zako->target.x = x + 10.0f;
                        zako->target.y = y;
                        zako->target.z = z + 10.0f;
                        break;
                    case 3:
                        zako->target.x = x + 45.0f;
                        zako->target.y = y;
                        zako->target.z = z - 5.0f;
                        break;
                    case 4:
                        zako->target.x = x + 55.0f;
                        zako->target.y = y;
                        zako->target.z = z + 5.0f;
                        break;
                    case 5:
                        zako->target.x = x + 75.0f;
                        zako->target.y = y;
                        zako->target.z = z;
                        break;
                }
                zako->state = 15;
                zako->timer = zako->timerMax = irand(40) + 40;
                zakogun_set_pose(zako, 0x16, 10, 1);
                break;
            case 15:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y = intplGetValue(0, zako->start.y, zako->target.y, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                if (i == 5) {
                    if (zako->timer % 12 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                    }
                    if (zako->timer % 12 == 6) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                    }
                }
                zako->timer--;
                if (zako->timer < 0) {
                    zako->timer = -1;
                    zako->rot.y = 0.0f;
                    if (i >= 3 && i <= 5) {
                        switch (i) {
                            case 3:
                            case 4:
                                for (j = 0, n = 0; j < 3; j++) {
                                    if (zakogun[j].state == 20) {
                                        n++;
                                    }
                                }
                                if (n == 3) {
                                    zako->state = 16;
                                }
                                break;
                            case 5:
                                for (j = 3, n = 0; j < 5; j++) {
                                    if (zakogun[j].state == 20) {
                                        n++;
                                    }
                                }
                                if (n == 2) {
                                    zako->state = 16;
                                }
                                break;
                        }
                    } else {
                        zako->state = 20;
                        zako->timer = 0;
                        zakogun_set_pose(zako, 0x15, 1, 1);
                    }
                }
                break;
            case 16:
                x = unit->mHomePosition.x;
                y = unit->mHomePosition.y;
                z = unit->mHomePosition.z;
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                switch (i) {
                    case 3:
                        zako->target.x = x - 5.0f;
                        zako->target.y = y + 10.0f;
                        zako->target.z = z - 5.0f;
                        break;
                    case 4:
                        zako->target.x = x + 5.0f;
                        zako->target.y = y + 10.0f;
                        zako->target.z = z + 5.0f;
                        break;
                    case 5:
                        zako->target.x = x;
                        zako->target.y = y + 20.0f;
                        zako->target.z = z;
                        break;
                }
                zako->state = 17;
                zako->timer = zako->timerMax = 20;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -200.0f) / 40.0f;
                zakogun_set_pose(zako, 0x22, 1, 1);
                break;
            case 17:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 20;
                    zako->timer = 0;
                    zako->pos.y = zako->target.y;
                    zakogun_set_pose(zako, 0x15, 1, 1);
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_WAKE1", 0x7f, 0, &zako->pos);
                }
                break;
            case 20:
                if (unit == NULL) {
                    break;
                }
                BtlUnit_GetPos(unit, &x, &y, &z);
                zako->timer++;
                zako->timer = zakogun[0].timer;
                if (zako->timer >= 30) {
                    zako->timer -= 30;
                }
                switch (i) {
                    case 0:
                        zako->pos.x = zako->home.x = x - 10.0f;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z - 10.0f;
                        break;
                    case 1:
                        zako->pos.x = zako->home.x = x;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z;
                        break;
                    case 2:
                        zako->pos.x = zako->home.x = x + 10.0f;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z + 10.0f;
                        break;
                    case 3:
                        zako->pos.x = zako->home.x = x - 5.0f;
                        if (zako->timer < 10) {
                            zako->pos.y = zako->home.y = y + 10.0f - 0.1f * zako->timer;
                        } else if (zako->timer < 20) {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f;
                        } else {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f + 0.1f * (zako->timer - 20.0f);
                        }
                        zako->pos.z = zako->home.z = z - 5.0f;
                        break;
                    case 4:
                        zako->pos.x = zako->home.x = x + 5.0f;
                        if (zako->timer < 10) {
                            zako->pos.y = zako->home.y = y + 10.0f - 0.1f * zako->timer;
                        } else if (zako->timer < 20) {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f;
                        } else {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f + 0.1f * (zako->timer - 20.0f);
                        }
                        zako->pos.z = zako->home.z = z + 5.0f;
                        break;
                    case 5:
                        zako->pos.x = zako->home.x = x;
                        if (zako->timer < 15) {
                            zako->pos.y = zako->home.y = y + 20.0f - 0.15f * zako->timer;
                        } else {
                            zako->pos.y = zako->home.y = y + 20.0f - 2.25f + 0.15f * (zako->timer - 15.0f);
                        }
                        zako->pos.z = zako->home.z = z;
                        break;
                }
                break;
            case 30:
                if (i >= 0 && i <= 2) {
                    zakogun_set_pose(zako, 0x14, 1, 1);
                }
                zako->state = 35;
                zako->timer = 5;
            case 35:
                if (i >= 3 && i <= 5) {
                    zako->pos.y += 3.0f;
                }
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 36;
                    zako->timer = 30;
                }
                break;
            case 36:
                zako->timer--;
                if (i >= 0 && i <= 2) {
                    if (zako->timer == 15) {
                        zakogun_set_pose(zako, 0x16, 10, 1);
                    }
                    if (zako->timer <= 15) {
                        if (zako->timer % 12 == 0) {
                            BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                        }
                        if (zako->timer % 12 == 6) {
                            BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                        }
                    }
                }
                if (zako->timer <= 0) {
                    zako->state = 40;
                }
                break;
            case 40:
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                x += 100.0f;
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                zako->target.x = x;
                zako->target.y = zako->pos.y;
                zako->target.z = zako->pos.z;
                zako->state = 45;
                zako->timer = zako->timerMax = 40;
            case 45:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                if (zako->timer % 12 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                }
                if (zako->timer % 12 == 6) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                }
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 50;
                    zako->timer = 0;
                }
                break;
            case 50: {
                f32 ofs[6] = {-20.0f, -10.0f, 0.0f, 10.0f, 20.0f, 30.0f};

                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                zako->target.x = x - ofs[i];
                zako->target.y = 0.0f;
                zako->target.z = zako->start.z;
                zako->state = 55;
                zako->timer = zako->timerMax = 40;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -320.0f) / 80.0f;
                zakogun_set_pose(zako, 0x20, 1, 1);
                if (i == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_ATTACK1", 0x7f, 0, &zako->pos);
                }
                break;
            }
            case 55:
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.2f;
                zako->timer--;
                if (zako->timer <= 10) {
                    zako->state = 58;
                    if (i == 0) {
                        unit->work[0]++;
                        zakogun_set_pose(zako, 0, 20, 2);
                    }
                }
                break;
            case 58:
                zako->pos.x = intplGetValue(0, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(0, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.2f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 60;
                    zako->timer = 30;
                    zako->pos.y = zako->target.y;
                    zakogun_set_pose(zako, 0x15, 1, 1);
                    if (i == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_ATTACK1_2", 0x7f, 0, &zako->pos);
                    }
                }
                break;
            case 60:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 62;
                    zakogun_set_pose(zako, 0x16, 10, 1);
                }
                break;
            case 62:
                zako->state = 12;
                zako->rot.y = 180.0f;
                zako->start = zako->pos;
                unit->mPosition = unit->mHomePosition;
                btl_camera_set_mode(0, 0);
                break;
            case 80:
                zako->rot.z = 0.0f;
                zako->state = 81;
                zako->timer = 6;
            case 81:
                if (i >= 0 && i <= 2) {
                    zako->rot.z += 15.0f;
                    if (zako->rot.z >= 45.0f) {
                        zako->rot.z = 45.0f;
                    }
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 82;
                    zako->timer = 10;
                    zako->vel.y = 0.0f;
                    if (i >= 0 && i <= 2) {
                        zako->rot.z = 45.0f;
                    }
                }
                break;
            case 82:
                if (i >= 3 && i <= 4) {
                    zako->rot.z += 15.0f;
                    if (zako->rot.z >= 45.0f) {
                        zako->rot.z = 45.0f;
                    }
                    zako->pos.y += zako->vel.y;
                    zako->vel.y += -0.5f;
                    if (zako->pos.y < 0.0f) {
                        zako->pos.y = 0.0f;
                    }
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 83;
                    zako->timer = 20;
                    zako->vel.y = 0.0f;
                    if (i >= 3 && i <= 4) {
                        zako->rot.z = 45.0f;
                        zako->pos.y = 0.0f;
                    }
                }
                break;
            case 83:
                if (i == 5) {
                    zako->rot.z += 15.0f;
                    if (zako->rot.z >= 45.0f) {
                        zako->rot.z = 45.0f;
                    }
                    zako->pos.y += zako->vel.y;
                    zako->vel.y += -0.5f;
                    if (zako->pos.y < 0.0f) {
                        zako->pos.y = 0.0f;
                    }
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->rot.y = 0.0f;
                    zako->state = 84;
                    zako->timer = 48;
                    if (i == 5) {
                        zako->rot.z = 45.0f;
                        zako->pos.y = 0.0f;
                    }
                }
                break;
            case 84:
                if (zako->timer <= 48) {
                    zako->rot.y -= 15.0f;
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->rot.y = 0.0f;
                    zako->state = 85;
                }
                break;
            case 85:
                zako->rot.x += 3.0f;
                if (zako->rot.x >= 90.0f) {
                    zako->state = -1;
                    zako->pos = (Vec){0.0f, -1000.0f, 0.0f};
                }
                break;
            case 90:
                zako->pos.x += 5.0f;
                if (i == 0) {
                    if (gp->retraceCount % 12 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                    }
                    if (gp->retraceCount % 12 == 6) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                    }
                }
                if (zako->pos.x >= 300.0f) {
                    zako->state = -1;
                }
                break;
            case 95:
            case 96:
                d = 0.0f;
                BtlUnit_GetPos(unit, &x, &y, &z);
                if (zako->state == 96) {
                    zako->timer++;
                    zako->timer = zakogun[0].timer;
                    if ((zako->timer & 3) == 0 || (zako->timer & 3) == 1) {
                        d = 1.0f;
                    } else {
                        d = -1.0f;
                    }
                }
                switch (i) {
                    case 0:
                        zako->pos.x = zako->home.x = x - 10.0f + d;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z - 10.0f;
                        break;
                    case 1:
                        zako->pos.x = zako->home.x = x + d;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z;
                        break;
                    case 2:
                        zako->pos.x = zako->home.x = x + 10.0f + d;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z + 10.0f;
                        break;
                    case 3:
                        zako->pos.x = zako->home.x = x - 5.0f + d;
                        zako->pos.y = zako->home.y = y + 20.0f;
                        zako->pos.z = zako->home.z = z - 5.0f;
                        break;
                    case 4:
                        zako->pos.x = zako->home.x = x + 5.0f + d;
                        zako->pos.y = zako->home.y = y + 20.0f;
                        zako->pos.z = zako->home.z = z + 5.0f;
                        break;
                    case 5:
                        zako->pos.x = zako->home.x = x + d;
                        zako->pos.y = zako->home.y = y + 40.0f;
                        zako->pos.z = zako->home.z = z;
                        break;
                }
                break;
        }
        PSMTXIdentity(*pose->mtx);
        PSMTXRotRad(m, 'x', 0.017453292f * zako->rot.x);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        PSMTXRotRad(m, 'y', 0.017453292f * zako->rot.y);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        PSMTXRotRad(m, 'z', 0.017453292f * zako->rot.z);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        (*pose->mtx)[0][3] = zako->pos.x;
        (*pose->mtx)[1][3] = zako->pos.y;
        (*pose->mtx)[2][3] = zako->pos.z + i * 0.25f;
    }
}

USER_FUNC(zakogun_attack1) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 6; i++) {
            zakogun[i].state = 30;
        }
    }
    for (i = 0, n = 0; i < 6; i++) {
        if (zakogun[i].state == 20) {
            n++;
        }
    }
    return n == 6 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_dead1) {
    s32 i;

    for (i = 0; i < 6; i++) {
        zakogun[i].state = 80;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(zakogun_escape1) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 6; i++) {
            ZakoGun* zako = &zakogun[i];
            zako->state = 90;
            zakogun_set_pose(zako, 0x16, 10, 1);
        }
    }
    for (i = 0, n = 0; i < 6; i++) {
        if (zakogun[i].state == -1) {
            n++;
        }
    }
    return n == 6 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(zakogun_start_sukima) {
    s32 mode = evtGetValue(event, event->args[0]);
    s32 i;

    for (i = 0; i < 6; i++) {
        ZakoGun* zako = &zakogun[i];
        if (zako->state < 80 || zako->state >= 90) {
            if (mode == 1) {
                zako->state = 96;
                zako->timer = 0;
            } else {
                zako->state = 95;
            }
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(zakogun_end_sukima) {
    s32 i;

    for (i = 0; i < 6; i++) {
        ZakoGun* zako = &zakogun[i];
        if (zako->state < 80 || zako->state >= 90) {
            zako->state = 20;
        }
    }
    return EVT_RETURN_DONE;
}

ZakoGunWork* work;
ZakoGun* zakogun;

