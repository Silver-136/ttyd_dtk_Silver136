#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

static USER_FUNC(_check_zenei_kouei);
extern s32 R_defence_attr_turn[];
extern s32 R_defence_turn[];
extern s32 R_regist_turn[];
extern PoseTableEntry gesso_pose_table_turn[];

BattleUnitKind unit_boss_gesso_right_arm;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
BattleDataEntry gesso_right_arm_data_table[13];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
static EvtWord init_event[25];
static EvtWord damage_event[31];
EvtWord gesso_right_arm_damage_event[93];
static EvtWord phase_event[2];
BattleWeapon weapon_gesso_right_arm_front_attack;
BattleWeapon weapon_gesso_right_arm_rear_attack;
static EvtWord attack_event[273];
static EvtWord wait_event[6];
EvtWord gesso_arm_dead_event[130];
EvtWord gesso_fall_check[219];
EvtWord gesso_arm_damage_pose_event[35];

BattleUnitKind unit_boss_gesso_right_arm = {
    10,
    "btl_un_gesso_right_arm",
    3,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    9,
    60,
    110,
    -68,
    -100,
    0.0f,
    0.0f,
    0.0f,
    -85,
    -140,
    0.0f,
    0.0f,
    0.0f,
    30.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    30.0f,
    0.0f,
    0.0f,
    30.0f,
    71.5f,
    0.0f,
    -60.0f,
    -60.0f,
    0.0f,
    60.0f,
    100.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_GESSO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x200000,
    &regist,
    1,
    parts,
    init_event,
    gesso_right_arm_data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {28, "GSO_S1_1"},
    {31, "GSO_S1_1"},
    {39, "GSO_D1_1"},
    {69, "GSO_S1_1"},
};

BattleDataEntry gesso_right_arm_data_table[] = {
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, gesso_right_arm_damage_event},
};

static BattleDataEntry data_table[] = {
    {10, gesso_arm_damage_pose_event},
    {UNIT_DATA_DEAD_EVENT, gesso_arm_dead_event},
    {0},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gesso_right_arm", "c_gesso2", 0.0f, 0.0f, 0.0f, 85.0f, -110.0f, 0.0f, 85.0f, -110.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, VERSION_PICK(0x680009, 0x2680009), 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnAttribute, -2, kUnitHidden)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_GetUnitWork, LW(3), 4, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(gesso_right_arm_damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
    SET_VALUE(LW(14), 39)
    SET_VALUE(LW(15), -1)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_SURPRISE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 38)
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
    END_SIBLING_EVENT
    CALL_CHILD(PTR(subsetevt_damage))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        WAIT_FRAMES(30)
        CALL_CHILD(PTR(subsetevt_dead))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    LABEL(99)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_GetUnitWork, LW(3), 4, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

BattleWeapon weapon_gesso_right_arm_front_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
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

BattleWeapon weapon_gesso_right_arm_rear_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    8, // targetWeightFlags
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

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(evt_sub_random, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&weapon_gesso_right_arm_front_attack))
    ELSE
        SET_VALUE(LW(12), PTR(&weapon_gesso_right_arm_rear_attack))
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    SET_VALUE(LW(11), 0)
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), 222)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(9))
        CALL(btlevtcmd_GetUnitWork, LW(9), 5, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetUnitWork, LW(9), 5, 1)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(LW(11), 0)
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(9))
        CALL(evt_msg_print, 2, STRING("tik_02_gs_04_1"), 0, LW(9))
        CALL(evt_msg_print, 2, STRING("tik_02_gs_04_2"), 0, LW(9))
        CALL(evt_msg_select, 0, STRING("tik_02_gs_04_3"))
        CALL(evt_msg_continue)
        IF_INT_EQUAL(LW(0), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_msg_print, 2, STRING("tik_02_gs_04_3_y"), 0, LW(9))
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_INLINE_EVENT
        ELSE
            BEGIN_INLINE_EVENT
                CALL(evt_msg_toge, 2, 110, 120, 30)
                CALL(evt_msg_print, 2, STRING("tik_02_gs_04_3_n"), 0, LW(9))
                CALL(btlevtcmd_StatusWindowOnOff, 1)
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(_check_zenei_kouei, LW(3), LW(13))
    IF_INT_EQUAL(LW(13), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_A1_1"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_A1_3"))
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 32, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_ARM_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftof, 55, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_ftof, 97, LW(0))
#else
    CALL(btlevtcmd_ftof, 94, LW(0))
#endif
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        SWITCH(LW(5))
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_SWITCH
    ELSE
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

EVT_BEGIN(gesso_arm_dead_event)
    CALL(btlevtcmd_GetUnitKind, -2, LW(0))
    IF_INT_EQUAL(LW(0), 10)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_ARM_GOOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_F1_1"))
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 2, -1)
        END_IF
        WAIT_FRAMES(50)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 2, -1)
        END_IF
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GESSO_ARM_GOOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GSO_F1_1"))
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 1, -1)
        END_IF
        WAIT_FRAMES(80)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 1, -1)
        END_IF
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), 4, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    CALL_CHILD(gesso_fall_check)
    LABEL(99)
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()

EVT_BEGIN(gesso_fall_check)
    CALL(btlevtcmd_GetUnitWork, LW(3), 1, LW(0))
    CALL(btlevtcmd_GetUnitId, LW(0), LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), 2, LW(0))
    CALL(btlevtcmd_GetUnitId, LW(0), LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), 4, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, LW(3), 4, 2)
    CALL(btlevtcmd_GetUnitWork, LW(3), 3, 0)
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F_2"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 16, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
    CALL(btlevtcmd_OffAttribute, LW(3), 2)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 6291456)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(3), 1, PTR(gesso_pose_table_turn))
    CALL(btlevtcmd_SetHpGaugeOffset, LW(3), 0, 0)
    CALL(btlevtcmd_SetStatusIconOffset, LW(3), 46, 118)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(3), 1, PTR(R_defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(3), 1, PTR(R_defence_attr_turn))
    CALL(btlevtcmd_SetRegistStatus, LW(3), PTR(R_regist_turn))
    CALL(btlevtcmd_SetHitOffset, LW(3), 1, -15, -90, 0)
    CALL(btlevtcmd_SetStatusIconOffset, -2, 55, 113)
    CALL(evt_btl_camera_shake_h, 0, 6, 0, 30, 4)
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, 1)
    CALL(btlevtcmd_SetHitOffset, LW(3), 1, 51, 38, 30)
    CALL(btlevtcmd_SetHitCursorOffset, LW(3), 1, 51, 38, 30)
    CALL(btlevtcmd_SetUnitWork, LW(3), 4, 0)
    CALL(btlevtcmd_StartWaitEvent, LW(3))
    RETURN
EVT_END()

EVT_BEGIN(gesso_arm_damage_pose_event)
    CALL(btlevtcmd_GetUnitKind, -2, LW(0))
    IF_INT_EQUAL(LW(0), 10)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        ELSE
        IF_INT_EQUAL(LW(0), 9)
            CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
            ELSE
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), 1, LW(14))
    RETURN
EVT_END()

static USER_FUNC(_check_zenei_kouei) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    f32 x, y, z;

    id = BattleTransID(event, id);
    BtlUnit_GetPos(BattleGetUnitPtr(wp, id), &x, &y, &z);
    if (x >= -120.0f) {
        evtSetValue(event, out, 0);
    } else {
        evtSetValue(event, out, 1);
    }
    return EVT_RETURN_DONE;
}
