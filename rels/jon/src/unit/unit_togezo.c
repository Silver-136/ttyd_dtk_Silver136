#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"

USER_FUNC(btlevtcmd_SetJumpSound);

BattleUnitKind unit_togezo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitDefense defence_turn;
static BattleUnitDefenseAttr defence_attr_turn;
static StatusVulnerability regist_turn;
static BattleUnitDefense defence_guard;
static BattleUnitDefenseAttr defence_attr_guard;
static StatusVulnerability regist_guard;
static PoseTableEntry pose_table[15];
static PoseTableEntry pose_table_turn[14];
static PoseTableEntry pose_table_guard[12];
static BattleDataEntry data_table[5];
static BattleDataEntry data_table_turn[3];
static BattleDataEntry data_table_guard[4];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togezo_attack;
static EvtWord init_event[52];
static EvtWord dmg_avoid_counter_thorn_event[61];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[84];
static EvtWord wakeup_event[90];
static EvtWord attack_event[71];
static EvtWord normal_attack_event[813];
static EvtWord guard_event[95];
static EvtWord guard_cancel_event[89];
static EvtWord wait_event[6];

BattleUnitKind unit_togezo = {
    41,
    "btl_un_togezo",
    3,
    0,
    1,
    1,
    9,
    0,
    1,
    70,
    0,
    50,
    9999,
    32,
    30,
    4,
    30,
    0.0f,
    15.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    0.0f,
    12.0f,
    16.0f,
    0.0f,
    0.0f,
    15.0f,
    0.0f,
    32.0f,
    30.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_TOGEZO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{3, 3, 3, 3, 3}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {80, 90, 80, 100, 80, 100, 100, 70, 100, 90, 100, 90, 100, 95, 90, 100, 30, 100, 90, 100, 100, 100};

static BattleUnitDefense defence_turn = {0};

static BattleUnitDefenseAttr defence_attr_turn = {0};

static StatusVulnerability regist_turn = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

static BattleUnitDefense defence_guard = {{99, 99, 99, 99, 99}};

static BattleUnitDefenseAttr defence_attr_guard = {{2, 2, 2, 2, 2}};

static StatusVulnerability regist_guard = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

static PoseTableEntry pose_table[] = {
    {1, "TGZ_N_1"},
    {2, "TGZ_Y_1"},
    {9, "TGZ_Y_1"},
    {5, "TGZ_K_1"},
    {4, "TGZ_X_1"},
    {3, "TGZ_X_1"},
    {28, "TGZ_S_1"},
    {29, "TGZ_Q_1"},
    {30, "TGZ_Q_1"},
    {31, "TGZ_A_1"},
    {39, "TGZ_D_1"},
    {40, "TGZ_W_1"},
    {42, "TGZ_R_1"},
    {65, "TGZ_T_1"},
    {69, "TGZ_S_1"},
};

static PoseTableEntry pose_table_turn[] = {
    {1, "TGZ_N_2"},
    {2, "TGZ_Y_2"},
    {9, "TGZ_Y_2"},
    {5, "TGZ_K_2"},
    {4, "TGZ_X_2"},
    {3, "TGZ_X_2"},
    {28, "TGZ_S_2"},
    {29, "TGZ_Q_2"},
    {30, "TGZ_Q_2"},
    {31, "TGZ_S_1"},
    {39, "TGZ_D_3"},
    {40, "TGZ_W_1"},
    {42, "TGZ_R_1"},
    {69, "TGZ_S_2"},
};

static PoseTableEntry pose_table_guard[] = {
    {1, "TGZ_P_1"},
    {2, "TGZ_P_1"},
    {9, "TGZ_P_1"},
    {5, "TGZ_P_1"},
    {4, "TGZ_P_1"},
    {3, "TGZ_P_1"},
    {28, "TGZ_P_1"},
    {29, "TGZ_P_1"},
    {30, "TGZ_P_1"},
    {31, "TGZ_P_1"},
    {39, "TGZ_P_1"},
    {69, "TGZ_P_1"},
};

static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {37, dmg_avoid_counter_thorn_event},
    {39, dmg_avoid_counter_thorn_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleDataEntry data_table_turn[] = {
    {13, dmg_turn_event},
    {48, btldefaultevt_Dummy},
    {0},
};

static BattleDataEntry data_table_guard[] = {
    {37, dmg_avoid_counter_thorn_event},
    {39, dmg_avoid_counter_thorn_event},
    {48, btldefaultevt_Dummy},
    {0},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_togezo", "c_togezo", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x1009, 0x5, pose_table},
};

static BattleWeapon weapon_togezo_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x2004, // targetWeightFlags
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
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_ENM_TOGEZO_MOVE1L"), STRING("SFX_ENM_TOGEZO_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_ENM_TOGEZO_MOVE1L"), STRING("SFX_ENM_TOGEZO_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 45)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 25)
        GOTO(90)
    END_IF
    ADD(LW(1), 35)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), 1)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), 1)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), 4)
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_turn))
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 39)
    CALL(evt_snd_sfxon, STRING("SFX_CONDITION_INSIDE"), 0)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    RETURN
EVT_END()

static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, LW(10))
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_OffStatusFlag, LW(10), 1)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), 1)
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), 4)
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 69)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, -2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(guard_cancel_event))
        GOTO(99)
        ELSE
        SET_VALUE(LW(0), 50)
        ADD(LW(0), 50)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 50)
            CALL_CHILD(PTR(normal_attack_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(guard_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_togezo_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togezo_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togezo_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_togezo_attack))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(14))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_TURN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 15, 0)
    LOOP(5)
        SET_VALUE(LW(0), -36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGZ_P_1"))
    LOOP(5)
        SET_VALUE(LW(0), -36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    SET_VALUE(LW(0), -36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(0))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
            CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_ROLL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 30, 0, FLOAT(0.14941406f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_ROLL2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, 0, 1)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_togezo_attack), 0, LW(5))
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_ROLL5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), -18)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_SetUnitWork, -2, 2, LW(0))
        SET_VALUE(LW(0), 1)
        MULTIPLY(LW(0), LW(14))
        LOOP(18)
            CALL(btlevtcmd_AddUnitWork, -2, 2, LW(0))
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 50)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, 0, 1)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 26)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    WAIT_FRAMES(10)
    GOTO(97)
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_ROLL4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togezo_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_togezo_attack), 256, LW(5))
    SET_VALUE(LW(0), 36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, -2, 2, -36)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 30)
    ADD(LW(2), 10)
    CALL(btlevtcmd_JumpSetting, -2, 30, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 30, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 30)
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpSetting, -2, 15, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), 18)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_SetUnitWork, -2, 2, LW(0))
        SET_VALUE(LW(0), -1)
        MULTIPLY(LW(0), LW(14))
        LOOP(18)
            CALL(btlevtcmd_AddUnitWork, -2, 2, LW(0))
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 18)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, 0, 1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 9)
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(0.5f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    WAIT_FRAMES(3)
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_LESS(LW(0), 30)
        GOTO(98)
    ELSE
        GOTO(97)
    END_IF
    LABEL(97)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 15, 0)
    LOOP(5)
        SET_VALUE(LW(0), 36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGZ_S_1"))
    LOOP(5)
        SET_VALUE(LW(0), 36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGZ_S_1"))
    WAIT_MS(250)
    GOTO(99)
    LABEL(98)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_ROLL3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    SET_VALUE(LW(0), 36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(0))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
            CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetPartsDefenceTable, -2, 1, PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, -2, 1, PTR(&defence_attr_guard))
    CALL(btlevtcmd_SetRegistStatus, -2, PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, -2, PTR(data_table_guard))
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kIsShelled)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    WAIT_MS(250)
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(guard_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_TURN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, 36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGZ_P_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, 36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_guard))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_guard))
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), 4096)
    CALL(btlevtcmd_SetUnitWork, LW(10), 0, 1)
    RETURN
EVT_END()

static EVT_BEGIN(guard_cancel_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGZ_S_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), 4096)
    CALL(btlevtcmd_SetUnitWork, LW(10), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

