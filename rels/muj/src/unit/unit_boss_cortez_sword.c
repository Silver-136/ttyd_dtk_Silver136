#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_AnimeWaitPlayLoopTiming);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventUnisonPhase);

BattleUnitKind unit_boss_cortez_sword;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
BattleWeapon weapon_cortez_sword_float_attack;
static PoseTableEntry pose_table[6];
PoseTableEntry pose_table_otitabuki[4];
static PoseTableEntry pose_table_break[4];
static BattleDataEntry data_table[21];
static BattleUnitKindPart parts[1];
static EvtWord init_event[47];
static EvtWord damage_event[10];
static EvtWord avoid_thorn_event[33];
EvtWord cortez_weapon_get_home_height[144];
EvtWord cortez_weapon_set_formation_attribute[27];
EvtWord cortez_weapon_unison_phase_event[79];
EvtWord cortez_weapon_phase_event[115];
static EvtWord attack_event[862];
static EvtWord wait_event[10];
static EvtWord cortez_damage_sub_event[25];
EvtWord cortez_weapon_counter_damage_event[113];
static EvtWord dead_event[2];
EvtWord cortez_weapon_dead_event[100];
EvtWord cortez_weapon_break_event[88];
EvtWord cortez_weapon_blow_dead_event[121];

BattleUnitKind unit_boss_cortez_sword = {
    UNIT_CORTEZ_SWORD, // mUnitType
    "btl_un_cortez_sword", // name
    4, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    20, // pbSoftCap
    130, // width
    30, // height
    {0, 30}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {65.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {65.0f, 0.0f, 0.0f}, // bintaHitOffset
    {65.0f, 19.5f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
    130.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_KOR_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating | kUnitNoHpGauge, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    80, // stop
    95, // dizzy
    100, // poison
    90, // confuse
    100, // electric
    100, // burn
    85, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    90, // defenseDown
    50, // allergic
    0, // fright
    90, // galeForce
    100, // fast
    50, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

BattleWeapon weapon_cortez_sword_float_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_STOP, "KRT_Z_1"},
    {POSE_TYPE_FREEZE, "KRT_Z_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_S_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

PoseTableEntry pose_table_otitabuki[] = {
    {POSE_TYPE_NORMAL, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE, "KRT_Z_1"},
    {POSE_TYPE_ACTION, "KRT_Z_1"},
};

static PoseTableEntry pose_table_break[] = {
    {POSE_TYPE_NORMAL, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE, "KRT_Z_1"},
    {POSE_TYPE_ACTION, "KRT_Z_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, cortez_damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, cortez_weapon_dead_event},
    {UNIT_DATA_WEAPON_BREAK_EVENT, cortez_weapon_break_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, cortez_weapon_blow_dead_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, cortez_weapon_counter_damage_event},
    {UNIT_DATA_AVOID_THORN_EVENT, avoid_thorn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, avoid_thorn_event},
    {UNIT_DATA_AVOID_THORN_FRONT_EVENT, avoid_thorn_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_cortez_sword", "c_buki2", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kHidden | kNoShadow, kCounterTopSpiky | kCounterPreemptiveFrontSpiky | kCounterFrontSpiky, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, 16777216)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(cortez_weapon_phase_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(cortez_weapon_unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(avoid_thorn_event)
    CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
    ADD(LW(2), 5)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_get_home_height)
    SET_VALUE(LW(5), 0)
    CALL(btlevtcmd_GetPos, LW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, LW(9), 0, LW(3))
    CALL(btlevtcmd_GetUnitWork, LW(3), 8, LW(4))
    SWITCH(LW(4))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetUnitWork, LW(9), 2, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            ADD(LW(5), 20)
            CASE_EQUAL(1)
            ADD(LW(5), 60)
            CASE_EQUAL(2)
            ADD(LW(5), 60)
            CASE_EQUAL(3)
            ADD(LW(5), 20)
        END_SWITCH
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetUnitWork, LW(9), 2, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            ADD(LW(5), 20)
            CASE_EQUAL(1)
            ADD(LW(5), 20)
            CASE_EQUAL(2)
            ADD(LW(5), 60)
            CASE_EQUAL(3)
            ADD(LW(5), 60)
        END_SWITCH
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetUnitWork, LW(9), 2, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            ADD(LW(5), 60)
            CASE_EQUAL(1)
            ADD(LW(5), 20)
            CASE_EQUAL(2)
            ADD(LW(5), 60)
            CASE_EQUAL(3)
            ADD(LW(5), 20)
        END_SWITCH
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetUnitWork, LW(9), 2, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            ADD(LW(5), 60)
            CASE_EQUAL(1)
            ADD(LW(5), 20)
            CASE_EQUAL(2)
            ADD(LW(5), 20)
            CASE_EQUAL(3)
            ADD(LW(5), 60)
        END_SWITCH
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_set_formation_attribute)
    CALL(btlevtcmd_GetBodyId, LW(9), LW(0))
    CALL(btlevtcmd_GetHomePos, LW(9), EVT_NULLPTR, LW(1), EVT_NULLPTR)
    IF_INT_GREATER_OR_EQUAL(LW(1), 40)
        CALL(btlevtcmd_OnPartsAttribute, LW(9), LW(0), 6291456)
    ELSE
        CALL(btlevtcmd_OffPartsAttribute, LW(9), LW(0), 6291456)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            RETURN
        END_IF
        CALL(btlevtcmd_GetUnitWork, LW(0), 5, LW(1))
        IF_INT_GREATER_OR_EQUAL(LW(1), 4)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(9))
                IF_INT_EQUAL(LW(9), -1)
                    RETURN
                END_IF
                CALL_CHILD(PTR(cortez_weapon_get_home_height))
                IF_INT_NOT_EQUAL(LW(1), LW(5))
                    CALL(btlevtcmd_PhaseEventStartDeclare, -2)
                    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 60, 0, 4, 0, -1)
                    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(5), LW(2))
                    CALL_CHILD(PTR(cortez_weapon_set_formation_attribute))
                    CALL(btlevtcmd_StartWaitEvent, -2)
                    RETURN
                END_IF
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                CALL(btlevtcmd_PhaseEventStartDeclare, -2)
                BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                    CALL(evt_sub_intpl_init, 5, -90, 0, 20)
                    CALL(evt_sub_intpl_get_value)
                    LOOP(20)
                        CALL(evt_sub_intpl_get_value)
                        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), 0, 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_SIBLING_EVENT
                CALL(btlevtcmd_WaitEventEnd, LW(0))
                CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(9))
                CALL_CHILD(PTR(cortez_weapon_get_home_height))
                CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 60, 0, 4, 0, -1)
                CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(5), LW(2))
                CALL_CHILD(PTR(cortez_weapon_set_formation_attribute))
                CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_ClearAllStatus, -2)
                CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
                CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitNoHpGauge)
                CALL(btlevtcmd_StartWaitEvent, -2)
                RETURN
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&weapon_cortez_sword_float_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_AnimeWaitPlayLoopTiming, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_Z_1"))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 48, 19, 0)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init, 0, 0, -30, 30)
        CALL(evt_sub_intpl_get_value)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(evt_sub_intpl_init, 2, 0, -55, 30)
        CALL(evt_sub_intpl_get_value)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(1), -20)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 105)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, -10, 4, 0, 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 52, 0, 1, 0, 0)
    END_SIBLING_EVENT
    CALL(evt_sub_random, 40, LW(5))
    ADD(LW(5), 12)
    WAIT_FRAMES(LW(5))
    DELETE_EVENT(LW(15))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init, 2, 0, 475, 30)
        CALL(evt_sub_intpl_get_value)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            ADD(LW(0), -55)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_DANCE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(1), -20)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 45)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 28, 0, 0, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(40)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(40)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(40)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(40)
        CASE_DEFAULT
        GOTO(41)
    END_SWITCH
    LABEL(40)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    GOTO(45)
    LABEL(41)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 0, LW(6))
    IF_INT_EQUAL(LW(6), 21)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
        ADD(LW(1), 15)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 5, -30, 0, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 5, 0, -30, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 21, 0, 5, 0, -1)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 4, -30, 0, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
        RETURN
    END_IF
    LABEL(45)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_MS(800)
    LABEL(50)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init, 2, 60, -225, 30)
        CALL(evt_sub_intpl_get_value)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init, 2, -30, -90, 20)
        CALL(evt_sub_intpl_get_value)
        LOOP(20)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 35)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -15, 5, 0, -1)
    WAIT_MS(300)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(evt_sub_intpl_init, 5, -225, 180, 30)
        CALL(evt_sub_intpl_get_value)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(1), -20)
        CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 60)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, 6, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_DANCE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    WAIT_FRAMES(7)
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_DEFAULT
        GOTO(91)
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 256, LW(6))
    IF_INT_EQUAL(LW(6), 21)
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
        ADD(LW(1), 15)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 5, 0, -30, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 21, 0, 5, 0, -1)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 4, -30, 0, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
        RETURN
    END_IF
    LABEL(95)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_MS(300)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init, 11, -90, 0, 10)
        CALL(evt_sub_intpl_get_value)
        LOOP(10)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_GetStageSize, LW(1), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 80)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(1))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(5.0f), LW(13))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(13), 0, 1, 0, -1)
    WAIT_MS(500)
    CALL(btlevtcmd_GetStageSize, LW(1), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 80)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), LW(1))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeWaitPlayLoopTiming, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(cortez_damage_sub_event)
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(11))
    CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("KRT_Z_1"))
    WAIT_FRAMES(30)
    LABEL(90)
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_counter_damage_event)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
    SWITCH(LW(0))
        CASE_EQUAL(43)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS3"), 0)
        SET_VALUE(LW(14), 36)
        CASE_OR_EQUAL(46)
        CASE_OR_EQUAL(49)
        CASE_OR_EQUAL(50)
        CASE_OR_EQUAL(54)
        SET_VALUE(LW(14), 29)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(14), 39)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 5, 0, 0, 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 20, -1)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 10, -1)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_dead_event)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    ADD(LW(1), 1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_FallPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init, 0, 0, -90, 15)
        CALL(evt_sub_intpl_get_value)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 15, -1)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 2)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_break))
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitNoHpGauge)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_break_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(0)
            CALL(btlevtcmd_SetUnitWork, LW(0), 1, -1)
            CASE_EQUAL(1)
            CALL(btlevtcmd_SetUnitWork, LW(0), 2, -1)
            CASE_EQUAL(2)
            CALL(btlevtcmd_SetUnitWork, LW(0), 3, -1)
            CASE_EQUAL(3)
            CALL(btlevtcmd_SetUnitWork, LW(0), 4, -1)
        END_SWITCH
    END_IF
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden | kNoShadow)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(cortez_weapon_blow_dead_event)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_DAMAGE)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, 2, 0, -1)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(0)
            CALL(btlevtcmd_SetUnitWork, LW(0), 1, -1)
            CASE_EQUAL(1)
            CALL(btlevtcmd_SetUnitWork, LW(0), 2, -1)
            CASE_EQUAL(2)
            CALL(btlevtcmd_SetUnitWork, LW(0), 3, -1)
            CASE_EQUAL(3)
            CALL(btlevtcmd_SetUnitWork, LW(0), 4, -1)
        END_SWITCH
    END_IF
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

