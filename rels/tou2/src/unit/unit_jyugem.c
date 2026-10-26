#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "tou2.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_SetEventUnisonPhase);
static USER_FUNC(togezo_copy_status);

BattleUnitKind unit_jyugem;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_hold[17];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[2];
static BattleWeapon weapon_jyugem_attack;
static BattleUnitSetup entry_togezo;
static EvtWord init_event[40];
static EvtWord damage_event[322];
static EvtWord unison_phase_event[139];
static EvtWord avoid_counter_thorn_head_event[61];
static EvtWord dmg_first_attack_pos_event[19];
static EvtWord attack_event[185];
static EvtWord normal_attack_event[585];
static EvtWord friend_event[263];
static EvtWord wait_event[6];

static USER_FUNC(togezo_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->mSizeChangeTurns = unit1->mSizeChangeTurns;
    unit2->mSizeChangeStrength = unit1->mSizeChangeStrength;
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_jyugem = {
    UNIT_LAKITU, // mUnitType
    "btl_un_jyugem", // name
    5, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    11, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    42, // height
    {4, 42}, // hitOffset
    {0.0f, 21.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 38.0f, 0.0f}, // kissHitOffset
    {0.0f, 21.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    42.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_JUGEMU_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitAirborne, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    80, // sleep
    90, // stop
    105, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    90, // fright
    100, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "JGM_N_1"},
    {POSE_TYPE_STOP, "JGM_Y_1"},
    {POSE_TYPE_FREEZE, "JGM_Y_1"},
    {POSE_TYPE_CONFUSE, "JGM_K_1"},
    {POSE_TYPE_POISON, "JGM_X_1"},
    {POSE_TYPE_DIZZY, "JGM_X_1"},
    {POSE_TYPE_NORMAL, "JGM_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "JGM_Q_1"},
    {30, "JGM_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "JGM_S_1"},
    {POSE_TYPE_DAMAGE, "JGM_D_1"},
    {POSE_TYPE_RUN, "JGM_R_1"},
    {POSE_TYPE_WALK, "JGM_W_1"},
    {POSE_TYPE_DANGER, "JGM_I_1"},
    {POSE_TYPE_PERIL, "JGM_I_1"},
    {POSE_TYPE_TALK, "JGM_T_1"},
    {POSE_TYPE_ACTION, "JGM_S_1"},
};

static PoseTableEntry pose_table_hold[] = {
    {POSE_TYPE_SLEEP, "JGM_S_2"},
    {POSE_TYPE_STOP, "JGM_S_2"},
    {POSE_TYPE_FREEZE, "JGM_S_2"},
    {POSE_TYPE_CONFUSE, "JGM_S_2"},
    {POSE_TYPE_POISON, "JGM_S_2"},
    {POSE_TYPE_DIZZY, "JGM_S_2"},
    {POSE_TYPE_NORMAL, "JGM_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "JGM_Q_1"},
    {30, "JGM_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "JGM_S_2"},
    {POSE_TYPE_DAMAGE, "JGM_D_1"},
    {POSE_TYPE_RUN, "JGM_S_2"},
    {POSE_TYPE_WALK, "JGM_S_2"},
    {POSE_TYPE_DANGER, "JGM_S_2"},
    {POSE_TYPE_PERIL, "JGM_S_2"},
    {POSE_TYPE_TALK, "JGM_S_2"},
    {POSE_TYPE_ACTION, "JGM_S_2"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_jyugem", "c_jugemu", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kPartNotHammerTarget | kPartNotShellTossTarget, 0, pose_table},
    {2, /*"パイポ"*/"\x83\x70\x83\x43\x83\x7C", "c_jugemu", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

static BattleWeapon weapon_jyugem_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

static BattleUnitSetup entry_togezo = {
    &unit_togezo, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("JGM_P_1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(btldefaultevt_Damage))
        ELSE
        CALL(btlevtcmd_GetDamage, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SET_VALUE(LW(15), 1)
            ELSE
            SET_VALUE(LW(15), 0)
        END_IF
        CALL(btlevtcmd_CheckStatus, LW(10), 27, LW(14))
        OR(LW(15), LW(14))
        CALL(btlevtcmd_CheckStatus, LW(10), 1, LW(14))
        OR(LW(15), LW(14))
        CALL(btlevtcmd_CheckStatus, LW(10), 3, LW(14))
        OR(LW(15), LW(14))
        CALL(btlevtcmd_CheckStatus, LW(10), 5, LW(14))
        OR(LW(15), LW(14))
        CALL(btlevtcmd_CheckStatus, LW(10), 6, LW(14))
        OR(LW(15), LW(14))
        CALL(btlevtcmd_CheckStatus, LW(10), 9, LW(14))
        OR(LW(15), LW(14))
        CALL(btlevtcmd_CheckStatus, LW(10), 2, LW(14))
        OR(LW(15), LW(14))
        IF_INT_NOT_EQUAL(LW(15), 0)
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                ADD(LW(1), 35)
                CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 200)
                CALL(btlevtcmd_JumpPartsSetting, LW(10), 2, 0, FLOAT(5.0f), FLOAT(0.2998047f))
                CALL(btlevtcmd_JumpPartsPosition, LW(10), 2, LW(0), 0, LW(2), 0, -1)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 2, 16777216)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 2, 33554432)
            END_INLINE_EVENT
            CALL(btlevtcmd_SetUnitWork, LW(10), 0, 0)
            CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), 1, 1)
            CALL(btlevtcmd_AnimeSetPoseTable, LW(10), 1, PTR(pose_table))
            CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
            CALL_CHILD(PTR(btldefaultevt_Damage))
            ELSE
            CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
            IF_INT_EQUAL(LW(0), 22)
                GOTO(90)
            END_IF
            IF_INT_EQUAL(LW(0), 30)
                GOTO(90)
            END_IF
            IF_INT_EQUAL(LW(0), 31)
                GOTO(90)
            END_IF
            IF_INT_EQUAL(LW(0), 38)
                GOTO(90)
            END_IF
            IF_INT_EQUAL(LW(0), 39)
                GOTO(90)
            END_IF
            IF_INT_EQUAL(LW(0), 41)
                GOTO(90)
            END_IF
            CALL(btlevtcmd_CheckStatus, LW(10), 4, LW(15))
            CALL(btlevtcmd_CheckStatus, LW(10), 8, LW(14))
            OR(LW(15), LW(14))
            CALL(btlevtcmd_CheckStatus, LW(10), 11, LW(14))
            OR(LW(15), LW(14))
            CALL(btlevtcmd_CheckStatus, LW(10), 15, LW(14))
            OR(LW(15), LW(14))
            CALL(btlevtcmd_CheckStatus, LW(10), 20, LW(14))
            OR(LW(15), LW(14))
            CALL(btlevtcmd_CheckStatus, LW(10), 0, LW(14))
            OR(LW(15), LW(14))
            IF_INT_NOT_EQUAL(LW(15), 0)
                BEGIN_INLINE_EVENT
                    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
                    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    ADD(LW(1), 35)
                    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_JumpPartsSetting, LW(10), 2, 30, 0, FLOAT(0.099609375f))
                    CALL(btlevtcmd_JumpPartsPosition, LW(10), 2, LW(0), LW(1), LW(2), 30, -1)
                    CALL(btlevtcmd_OnPartsAttribute, LW(10), 2, 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, LW(10), 2, 33554432)
                END_INLINE_EVENT
            END_IF
            LABEL(90)
            CALL_CHILD(PTR(btldefaultevt_Damage))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_ELECTRIC, LW(14))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_LESS(LW(0), 50)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_1"))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_HOLD1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_2"))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
        CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, 1)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_hold))
        CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 20, 0)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 45)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
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

static EVT_BEGIN(dmg_first_attack_pos_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        GOTO(99)
    END_IF
    SET_VALUE(LW(11), 20)
    ADD(LW(11), 65)
    ADD(LW(11), 15)
    SUBTRACT(LW(11), 1)
    CALL(evt_sub_random, LW(11), LW(10))
    SET_VALUE(LW(11), 20)
    IF_INT_LESS(LW(10), LW(11))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(1), 0)
        LOOP(0)
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), 0)
                SUBTRACT(LW(2), 10)
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                IF_INT_EQUAL(LW(3), -1)
                    CALL_CHILD(PTR(friend_event))
                    GOTO(99)
                END_IF
                ELSE
                GOTO(9)
            END_IF
        END_LOOP
        LABEL(9)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(1), 0)
        LOOP(0)
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
            IF_INT_LESS_OR_EQUAL(LW(0), 150)
                ADD(LW(2), 10)
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                IF_INT_EQUAL(LW(3), -1)
                    CALL_CHILD(PTR(friend_event))
                    GOTO(99)
                END_IF
                ELSE
                GOTO(10)
            END_IF
        END_LOOP
    END_IF
    LABEL(10)
    SET_VALUE(LW(11), 65)
    ADD(LW(11), 15)
    SUBTRACT(LW(11), 1)
    CALL(evt_sub_random, LW(11), LW(10))
    SET_VALUE(LW(11), 65)
    IF_INT_LESS(LW(10), LW(11))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
        ELSE
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 1)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_jyugem_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_jyugem_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_jyugem_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_jyugem_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(10))
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_1"))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_2"))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
        CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, 1)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_3"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 35)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_THROW1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 45, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 45, -1)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_jyugem_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, 2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_jyugem_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_jyugem_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckDamageCode, LW(3), DAMAGE_RESULT_GUARD, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SET_VALUE(LW(1), 0)
            LOOP(0)
                CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
                SET_VALUE(LW(4), LW(0))
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
                MULTIPLY(LW(4), LW(5))
                IF_INT_LESS(LW(4), 0)
                    SUBTRACT(LW(2), 10)
                    CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                    IF_INT_EQUAL(LW(3), -1)
                        GOTO(97)
                    END_IF
                ELSE
                    GOTO(96)
                END_IF
            END_LOOP
            LABEL(96)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SET_VALUE(LW(1), 0)
            LOOP(0)
                CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
                IF_INT_LESS_OR_EQUAL(LW(0), 150)
                    ADD(LW(2), 10)
                    CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                    IF_INT_EQUAL(LW(3), -1)
                        GOTO(97)
                    END_IF
                ELSE
                    GOTO(98)
                END_IF
            END_LOOP
        END_IF
    END_IF
    GOTO(98)
    LABEL(97)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_REBOUND1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 45, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 45, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 15, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_togezo), 0)
    CALL(togezo_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, LW(3), LW(4), STRING("TGZ_P_1"))
    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, LW(3), 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, LW(3), 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, LW(3), LW(4), STRING("TGZ_S_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, LW(3), 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 0, 0)
    GOTO(99)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 150)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(5.0f), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), 0, LW(2), 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
    WAIT_FRAMES(10)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(friend_event)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(10))
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_1"))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_2"))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
        CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, 1)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("JGM_A_3"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_THROW2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    ADD(LW(11), 35)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(10), LW(11), LW(12))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 45, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 45, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 15, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_JUGEM_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_togezo), 0)
    CALL(togezo_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, LW(3), LW(4), STRING("TGZ_P_1"))
    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, LW(3), 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, LW(3), 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, LW(3), LW(4), STRING("TGZ_S_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, LW(3), 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 0, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

