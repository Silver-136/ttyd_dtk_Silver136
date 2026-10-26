#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_SetJumpSound);

BattleUnitKind unit_gundan_zako;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[16];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_gundan_zako_attack;
static BattleWeapon weapon_gundan_zako_jump_attack;
static BattleWeapon weapon_gundan_zako_dekadeka_bottle;
static EvtWord init_event[42];
static EvtWord damage_event[10];
static EvtWord attack_event[98];
static EvtWord attack_common_event1[182];
static EvtWord attack_common_event2[33];
static EvtWord normal_attack_event[307];
static EvtWord jump_attack_event[324];
static EvtWord dekadeka_event[125];
static EvtWord wait_event[6];

BattleUnitKind unit_gundan_zako = {
    UNIT_X_NAUT, // mUnitType
    "btl_un_gundan_zako", // name
    4, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    13, // mLevel
    0, // mBonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    60, // runRate
    9999, // pbSoftCap
    30, // width
    36, // height
    {0, 36}, // hitOffset
    {0.0f, 18.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {15.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 27.0f, 0.0f}, // kissHitOffset
    {0.0f, 18.0f, 0.0f}, // cutBaseOffset
    30.0f, // cutWidth
    36.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_3RD1_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    90, // sleep
    90, // stop
    90, // dizzy
    100, // poison
    90, // confuse
    100, // electric
    100, // burn
    80, // freeze
    100, // huge
    95, // tiny
    100, // attackUp
    95, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    100, // fright
    80, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_KO, "Z_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "D_1"},
    {30, "D_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako", "c_zako_n", 0.0f, 0.0f, 0.0f, 0.0f, 28.0f, 0.0f, 0.0f, 38.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

static BattleWeapon weapon_gundan_zako_attack = {
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
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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

static BattleWeapon weapon_gundan_zako_jump_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    0x80002000, // targetWeightFlags
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

static BattleWeapon weapon_gundan_zako_dekadeka_bottle = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, // burnChance..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE1"), STRING("SFX_ENM_3RD1_MOVE2"), 0, 5, 5)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE1"), STRING("SFX_ENM_3RD1_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, 0, STRING("SFX_ENM_3RD1_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(10)
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 50)
    ADD(LW(0), 30)
    ADD(LW(0), 20)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 50)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 30)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(jump_attack_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL_CHILD(PTR(dekadeka_event))
            GOTO(99)
        END_IF
    END_IF
    GOTO(10)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_common_event1)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
    BEGIN_SIBLING_EVENT
        LOOP(9)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(5)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    LOOP(9)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
        ADD(LW(2), 1)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.5f), 10, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(10)
    END_LOOP
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.5f), 10, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, -1, 0)
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(2), 1)
    RETURN
EVT_END()

static EVT_BEGIN(attack_common_event2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_attack))
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(attack_common_event1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.19921875f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0)
    END_SIBLING_EVENT
    LOOP(0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
        IF_INT_EQUAL(LW(14), -1)
            IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
        ELSE
            IF_INT_GREATER_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
        LABEL(90)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_2"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, 1)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_3"))
        WAIT_FRAMES(10)
        GOTO(98)
        LABEL(91)
        CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_add_zoom, 0, -50)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 48)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(2.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("J_1B"))
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(20)
        GOTO(98)
        LABEL(98)
        CALL_CHILD(attack_common_event2)
        RETURN
EVT_END()

static EVT_BEGIN(jump_attack_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_jump_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(attack_common_event1)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(1.75f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
        LABEL(90)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("kemuri1_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_2"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, 1)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_3"))
        WAIT_FRAMES(10)
        GOTO(98)
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("kemuri1_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(0.5f), FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(0.5f), FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 3)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(0.5f), FLOAT(0.099609375f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0)
        WAIT_FRAMES(15)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(10)
        GOTO(98)
        LABEL(98)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL_CHILD(attack_common_event2)
        RETURN
EVT_END()

static EVT_BEGIN(dekadeka_event)
    SET_VALUE(LW(3), -2)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gundan_zako_dekadeka_bottle))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gundan_zako_dekadeka_bottle))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    WAIT_FRAMES(60)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4A"))
    WAIT_FRAMES(80)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        LOOP(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(20)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4B"))
    WAIT_FRAMES(120)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(55)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_INLINE_EVENT
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gundan_zako_dekadeka_bottle))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gundan_zako_dekadeka_bottle), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

