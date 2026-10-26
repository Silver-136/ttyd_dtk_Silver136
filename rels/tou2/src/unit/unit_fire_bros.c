#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

USER_FUNC(fire_bros_nagemakuri_set_target);
void fire_bros_get_next_target(s32* unitIdx, s32* partIdx);
void fire_bros_get_target(s32* unitIdx, s32* partIdx);

BattleUnitKind unit_fire_bros;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[7];
BattleWeapon weapon_fire_bros_attack;
BattleWeapon weapon_fire_bros_renzoku_attack;
BattleWeapon weapon_fire_bros_nagemakuri_attack;
static EvtWord init_event[21];
static EvtWord damage_event[10];
static EvtWord attack_event[92];
static EvtWord damage_check_sub[167];
static EvtWord damage_check_no_last[7];
static EvtWord damage_check_last[113];
EvtWord fire_move_event[605];
static EvtWord normal_attack_event[125];
static EvtWord renzoku_attack_event[141];
static EvtWord nagemakuri_attack_event[316];
EvtWord nagemakuri_judge_last_attack_event[73];
static EvtWord wait_event[6];

void fire_bros_get_next_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;

    wt->currentTarget++;
    if (wt->currentTarget < wt->numTargets) {
        BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];
        *unitIdx = target->unitIdx;
        *partIdx = target->partIdx;
    } else {
        *unitIdx = -1;
        *partIdx = 0;
    }
}

void fire_bros_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

USER_FUNC(fire_bros_nagemakuri_set_target) {
    EvtWord* args = event->args;
    s32 unitIdx[20];
    s32 partIdx[20];
    s32 i;
    s32 num;

    i = 0;
    _battleWorkPointer->weaponTargets.currentTarget = 0;
    fire_bros_get_target(&unitIdx[0], &partIdx[0]);
    while (TRUE) {
        if (unitIdx[i] == -1) {
            num = i;
            break;
        }
        i++;
        fire_bros_get_next_target(&unitIdx[i], &partIdx[i]);
    }
    for (i = 0; i < 5; i++) {
        s32 r = irand(num);
        evtSetValue(event, *args++, unitIdx[r]);
        evtSetValue(event, *args++, partIdx[r]);
    }
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_fire_bros = {
    UNIT_FIRE_BRO, // mUnitType
    "btl_un_fire_bros", // name
    7, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    20, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    36, // width
    44, // height
    {3, 44}, // hitOffset
    {0.0f, 22.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {18.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 28.0f, 0.0f}, // kissHitOffset
    {0.0f, 22.0f, 0.0f}, // cutBaseOffset
    36.0f, // cutWidth
    44.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_FIREB_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    7, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    80, // sleep
    80, // stop
    70, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    70, // fright
    60, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "BRO_N_1"},
    {POSE_TYPE_STOP, "BRO_Y_1"},
    {POSE_TYPE_FREEZE, "BRO_Y_1"},
    {POSE_TYPE_CONFUSE, "BRO_K_1"},
    {POSE_TYPE_POISON, "BRO_X_1"},
    {POSE_TYPE_DIZZY, "BRO_X_1"},
    {POSE_TYPE_KO, "BRO_D_1"},
    {POSE_TYPE_NORMAL, "BRO_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "BRO_Q_1"},
    {30, "BRO_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "BRO_S_1"},
    {POSE_TYPE_DAMAGE, "BRO_D_1"},
    {POSE_TYPE_RUN, "BRO_R_1"},
    {POSE_TYPE_WALK, "BRO_W_1"},
    {POSE_TYPE_DANGER, "BRO_X_1"},
    {POSE_TYPE_PERIL, "BRO_X_1"},
    {POSE_TYPE_TALK, "BRO_T_1"},
    {POSE_TYPE_ACTION, "BRO_S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {6, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {7, "btl_un_fire_bros", "c_burosu_f", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

BattleWeapon weapon_fire_bros_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    30, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_fire_bros_renzoku_attack = {
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
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    30, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_fire_bros_nagemakuri_attack = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    30, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
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
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 3)
        SET_VALUE(LW(0), 50)
        ADD(LW(0), 50)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 30)
            CALL_CHILD(PTR(renzoku_attack_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(nagemakuri_attack_event))
        GOTO(99)
        ELSE
        SET_VALUE(LW(0), 70)
        ADD(LW(0), 30)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 70)
            CALL_CHILD(PTR(normal_attack_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(renzoku_attack_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_sub)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), 0, LW(5))
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
    GOTO(99)
    LABEL(91)
    SET_VALUE(LW(0), 1)
    IF_INT_EQUAL(LW(6), PTR(&weapon_fire_bros_renzoku_attack))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, 10, 1)
    END_IF
    IF_INT_EQUAL(LW(8), 1)
        SET_VALUE(LW(13), 0)
        SET_VALUE(LW(14), 5)
        LOOP(0)
            SET_VALUE(LW(12), 2)
            LOOP(0)
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), LW(13), LW(1))
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), LW(14), LW(2))
                IF_INT_EQUAL(LW(3), LW(1))
                    IF_INT_EQUAL(LW(4), LW(2))
                        ADD(LW(13), 10)
                        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), LW(13), 1)
                        GOTO(98)
                    END_IF
                END_IF
                ADD(LW(12), 1)
                IF_INT_GREATER(LW(12), 7)
                    BREAK_LOOP
                END_IF
            END_LOOP
            ADD(LW(13), 1)
            ADD(LW(14), 1)
        END_LOOP
    END_IF
    LABEL(98)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(6))
    IF_INT_EQUAL(LW(7), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), DAMAGE_RESULT_FINAL_HIT, LW(5))
        ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), 0, LW(5))
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_no_last)
    SET_VALUE(LW(7), 0)
    CALL_CHILD(PTR(damage_check_sub))
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_last)
    IF_INT_EQUAL(LW(6), PTR(&weapon_fire_bros_renzoku_attack))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 2, 10, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            SET_VALUE(LW(0), 1)
            ELSE
            SET_VALUE(LW(0), 0)
        END_IF
        GOTO(10)
    END_IF
    IF_INT_EQUAL(LW(8), 1)
        SET_VALUE(LW(13), 0)
        SET_VALUE(LW(14), 5)
        LOOP(0)
            SET_VALUE(LW(12), 2)
            LOOP(0)
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), LW(13), LW(1))
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), LW(14), LW(2))
                IF_INT_EQUAL(LW(3), LW(1))
                    IF_INT_EQUAL(LW(4), LW(2))
                        ADD(LW(13), 10)
                        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), LW(13), LW(0))
                        GOTO(10)
                    END_IF
                END_IF
                ADD(LW(12), 1)
                IF_INT_GREATER(LW(12), 7)
                    BREAK_LOOP
                END_IF
            END_LOOP
            ADD(LW(13), 1)
            ADD(LW(14), 1)
        END_LOOP
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(6))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), DAMAGE_RESULT_FINAL_HIT, LW(5))
        ELSE
        SET_VALUE(LW(7), 1)
        CALL_CHILD(PTR(damage_check_sub))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(fire_move_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(15), 15, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, -180)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(15), 0, 0, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(9), LW(10), LW(11))
    SUBTRACT(LW(9), LW(0))
    SET_VALUE(LW(10), 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(12), LW(13))
            SWITCH(LW(10))
                CASE_EQUAL(0)
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(11), LW(12), LW(13))
                IF_INT_GREATER_OR_EQUAL(LW(9), 0)
                    IF_INT_LESS_OR_EQUAL(LW(0), LW(11))
                        BREAK_SWITCH
                    END_IF
                ELSE
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(11))
                        BREAK_SWITCH
                    END_IF
                END_IF
                IF_INT_EQUAL(LW(14), 0)
                    CALL_CHILD(damage_check_no_last)
                ELSE
                    CALL_CHILD(damage_check_last)
                END_IF
                IF_INT_EQUAL(LW(0), 1)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
                    SET_VALUE(LW(10), 2)
                ELSE
                    SET_VALUE(LW(10), 1)
                END_IF
                CASE_END
                CASE_EQUAL(1)
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(11), LW(12), LW(13))
                IF_INT_GREATER_OR_EQUAL(LW(11), 250)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
                    SET_VALUE(LW(10), 2)
                END_IF
                IF_INT_LESS_OR_EQUAL(LW(11), -250)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
                    SET_VALUE(LW(10), 2)
                END_IF
                CASE_END
                CASE_EQUAL(2)
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(15), 15, 1)
                BREAK_LOOP
                CASE_END
            END_SWITCH
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 16)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_sub_random, 1, LW(11))
    IF_INT_EQUAL(LW(11), 0)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, 6, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, 5, FLOAT(0.5996094f))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        IF_INT_EQUAL(LW(10), 2)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(0), 0, LW(2), 0, -1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(10))
        IF_INT_EQUAL(LW(10), 1)
            GOTO(99)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FIREB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    LABEL(99)
    WAIT_FRAMES(6)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 16777216)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), 33554432)
    CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, 1, 1)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_fire_bros_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_fire_bros_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_fire_bros_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_fire_bros_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_1A"))
    WAIT_FRAMES(40)
    SET_VALUE(LW(15), 2)
    SET_VALUE(LW(14), 1)
    SET_VALUE(LW(6), PTR(&weapon_fire_bros_attack))
    SET_VALUE(LW(8), 0)
    SPAWN_EVENT(PTR(fire_move_event))
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_FRAMES(20)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(renzoku_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_fire_bros_renzoku_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_fire_bros_renzoku_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_fire_bros_renzoku_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_fire_bros_renzoku_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, 10, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
    WAIT_FRAMES(20)
    SET_VALUE(LW(15), 2)
    SET_VALUE(LW(14), 0)
    SET_VALUE(LW(6), PTR(&weapon_fire_bros_renzoku_attack))
    SET_VALUE(LW(8), 0)
    SPAWN_EVENT(PTR(fire_move_event))
    WAIT_FRAMES(12)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
    WAIT_FRAMES(20)
    SET_VALUE(LW(15), 3)
    SET_VALUE(LW(14), 1)
    SET_VALUE(LW(6), PTR(&weapon_fire_bros_renzoku_attack))
    SET_VALUE(LW(8), 0)
    SPAWN_EVENT(PTR(fire_move_event))
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 2)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_FRAMES(20)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(nagemakuri_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_fire_bros_nagemakuri_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_fire_bros_nagemakuri_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_fire_bros_nagemakuri_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_fire_bros_nagemakuri_attack))
    SET_VALUE(LW(10), 2)
    LOOP(0)
        IF_INT_GREATER(LW(10), 7)
            BREAK_LOOP
        END_IF
        CALL(fire_bros_nagemakuri_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 0, LW(0))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 1, LW(2))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 2, LW(4))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 3, LW(6))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 4, LW(8))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 5, LW(1))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 6, LW(3))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 7, LW(5))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 8, LW(7))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 9, LW(9))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 10, 0)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 11, 0)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 12, 0)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 13, 0)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(10), 14, 0)
        ADD(LW(10), 1)
    END_LOOP
    WAIT_FRAMES(12)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    LOOP(0)
        SET_VALUE(LW(15), 2)
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(1), 6)
                LOOP(0)
                    WAIT_FRAMES(1)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(1))
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(2))
                    SUBTRACT(LW(1), 6)
                    IF_INT_GREATER(LW(2), LW(1))
                        WAIT_FRAMES(3)
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
            WAIT_FRAMES(20)
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), LW(10), LW(3))
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), LW(11), LW(4))
            CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(14))
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
            CALL_CHILD(PTR(nagemakuri_judge_last_attack_event))
            SET_VALUE(LW(6), PTR(&weapon_fire_bros_nagemakuri_attack))
            SET_VALUE(LW(8), 1)
            SPAWN_EVENT(PTR(fire_move_event))
            CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, 0, 1)
            WAIT_FRAMES(15)
            ADD(LW(15), 1)
            IF_INT_GREATER(LW(15), 7)
                BREAK_LOOP
            END_IF
        END_LOOP
        ADD(LW(10), 1)
        ADD(LW(11), 1)
        IF_INT_GREATER(LW(10), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 6)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

EVT_BEGIN(nagemakuri_judge_last_attack_event)
    SET_VALUE(LW(0), LW(10))
    SET_VALUE(LW(1), LW(11))
    SET_VALUE(LW(13), LW(15))
    ADD(LW(13), 1)
    LOOP(0)
        IF_INT_GREATER(LW(0), 0)
            SET_VALUE(LW(14), 1)
            GOTO(99)
        END_IF
        LOOP(0)
            IF_INT_GREATER(LW(13), 7)
                SET_VALUE(LW(13), 2)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(13), LW(0), LW(5))
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(13), LW(1), LW(6))
            IF_INT_EQUAL(LW(3), LW(5))
                IF_INT_EQUAL(LW(4), LW(6))
                    SET_VALUE(LW(14), 0)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(13), 1)
        END_LOOP
        ADD(LW(0), 1)
        ADD(LW(1), 1)
    END_LOOP
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

