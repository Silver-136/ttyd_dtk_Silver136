// unit_boomerang_bros: Boomerang Bro, battle unit.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_camera.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_RETURNED 0        // boomerangs back in hand: boomerang_move_event adds 1 at its end; the attack waits for 1 (normal) or 2 (renzoku)
#define UNIT_WORK_TARGET1 1         // unit id of the first target (-1 = none)
#define UNIT_WORK_TARGET2 2         // unit id of the second target (-1 = none)
#define UNIT_WORK_TARGET1_PART 3    // part id of the first target
#define UNIT_WORK_TARGET2_PART 4    // part id of the second target
#define UNIT_WORK_TARGET1_HIT 5     // 1 if the outbound pass hit the first target; the last boomerang's return pass then damages it without a new check (damage_check_last)
#define UNIT_WORK_TARGET2_HIT 6     // 1 if the outbound pass hit the second target (same use as UNIT_WORK_TARGET1_HIT)
#define UNIT_WORK_AC_DEFENCE_DONE 7 // 1 once btlevtcmd_ResultACDefence ran for the current pass; attack_event clears it and boomerang_move_event clears it when the boomerang turns back


BattleUnitKind unit_boomerang_bros;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[4];
BattleWeapon weapon_boomerang_bros_attack;
BattleWeapon weapon_boomerang_bros_renzoku_attack;
static EvtWord init_event[21];
static EvtWord damage_event[10];
static EvtWord attack_event[71];
static EvtWord damage_check_sub[107];
static EvtWord damage_check_no_last[7];
static EvtWord damage_check_last[38];
EvtWord boomerang_move_event[355];
static EvtWord normal_attack_event[174];
static EvtWord renzoku_attack_event[178];
EvtWord normal_attack_event_confuse[322];
EvtWord renzoku_attack_event_confuse[328];
static EvtWord wait_event[6];

/**
 * Unit kind of the Boomerang Bro (HP 7, level 20):
 * its size, offsets, damage sounds, status vulnerability, parts and init event.
 */
BattleUnitKind unit_boomerang_bros = {
    UNIT_BOOMERANG_BRO, // unitType
    "btl_un_boomerang_bros", // name
    7, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    20, // level
    0, // bonusExp
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
    "SFX_ENM_BOOME_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    4, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Boomerang Bro: 1 damage points off every hit. */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Element reactions of the Boomerang Bro: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of the Boomerang Bro (percent chance to take each status): resists most statuses. */
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {80, 80, 70, 100, 80, 100, 100, 70, 100, 80, 100, 80, 100, 80, 80, 40, 60, 100, 80, 100, 100, 95};
#else
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
#endif

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
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BRO_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "BRO_S_1"},
    {POSE_TYPE_DAMAGE, "BRO_D_1"},
    {POSE_TYPE_RUN, "BRO_R_1"},
    {POSE_TYPE_WALK, "BRO_W_1"},
    {POSE_TYPE_DANGER, "BRO_X_1"},
    {POSE_TYPE_PERIL, "BRO_X_1"},
    {POSE_TYPE_TALK, "BRO_T_1"},
    {POSE_TYPE_ACTION, "BRO_S_1"},
};

/** Data table of the Boomerang Bro: empty; it has no event hooks. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Parts of the Boomerang Bro (4 parts): hit positions, defences, attributes and pose tables. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_boomerang_bros", "c_burosu_b", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_boomerang_bros", "c_burosu_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_boomerang_bros", "c_burosu_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_boomerang_bros", "c_burosu_b", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/** Weapon of the Boomerang Bro's attack: 2 damage. */
BattleWeapon weapon_boomerang_bros_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Boomerang Bro's rapid-fire attack: 2 damage. */
BattleWeapon weapon_boomerang_bros_renzoku_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Init event of the Boomerang Bro: registers its wait, attack,
 * damage and confusion events and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event: runs the default battle damage reaction on the unit itself (part 1). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Attack event of the Boomerang Bro: clears unit work 7, uses an enemy item if one fits,
 * and otherwise throws boomerangs. A first strike always uses normal_attack_event.
 * With 3 HP or less it has a 30% chance of the two-throw renzoku_attack_event;
 * every other turn it uses normal_attack_event.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, 0)
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
        SET_VALUE(LW(0), 70)
        ADD(LW(0), 30)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 30)
            CALL_CHILD(PTR(renzoku_attack_event))
            GOTO(99)
        END_IF
    END_IF
    CALL_CHILD(PTR(normal_attack_event))
    GOTO(99)
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, LW(8))
    IF_INT_EQUAL(LW(8), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(6))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, 1)
    END_IF
    IF_INT_EQUAL(LW(7), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), DAMAGE_RESULT_FINAL_HIT, LW(5))
        ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), 0, LW(5))
    END_IF
    SET_VALUE(LW(0), 1)
    GOTO(99)
    LABEL(99)
    RETURN
EVT_END()

/** Checks a boomerang hit that is not the last one (LW(7) = 0): runs damage_check_sub. */
static EVT_BEGIN(damage_check_no_last)
    SET_VALUE(LW(7), 0)
    CALL_CHILD(PTR(damage_check_sub))
    RETURN
EVT_END()

/**
 * Checks the final boomerang hit: if an earlier pass already hit (LW(0) = 1) it applies the last hit directly,
 * otherwise it runs damage_check_sub with LW(7) = 1.
 */
static EVT_BEGIN(damage_check_last)
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, LW(8))
        IF_INT_EQUAL(LW(8), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(6))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, 1)
        END_IF
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(6), DAMAGE_RESULT_FINAL_HIT, LW(5))
        ELSE
        SET_VALUE(LW(7), 1)
        CALL_CHILD(PTR(damage_check_sub))
    END_IF
    RETURN
EVT_END()

/**
 * Flight of one boomerang (part LW(15), weapon LW(6)):
 * plays the throw sound, spins the part and sends it out to x = -200 and back to the thrower.
 * A sibling loop hits the first target (unit work 1 and 3) and
 * then the second (unit work 2 and 4) as the boomerang passes them on the way out,
 * keeping the results in unit work 5 and 6, and hits them again in reverse order on the way back,
 * the final check counting as the last hit when LW(14) is 1;
 * unit work 7, the guard-result flag, is cleared before the return hits.
 * Once back the part is hidden again and unit work 0 (boomerangs returned) goes up by one.
 */
EVT_BEGIN(boomerang_move_event)
    // The boomerang is thrown: its throw sound plays and it appears spinning above the Boomerang Bro's hand.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, -180)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(15), 0, 0, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kNoShadow)
    SET_VALUE(LW(9), 0)
    // As it flies out, it hits the first target it passes, then the second;
    // on the way back it hits them again in reverse order.
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(10), LW(12), LW(13))
            IF_INT_EQUAL(LW(9), 2)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, 0)
            END_IF
            SWITCH(LW(9))
                CASE_EQUAL(0)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1, LW(3))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_PART, LW(4))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(9), 1)
                    BREAK_SWITCH
                END_IF
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(11), LW(12), LW(13))
                IF_INT_GREATER_OR_EQUAL(LW(10), LW(11))
                    BREAK_SWITCH
                END_IF
                CALL_CHILD(damage_check_no_last)
                SET_VALUE(LW(9), 1)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_HIT, LW(0))
                CASE_END
                CASE_EQUAL(1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2, LW(3))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_PART, LW(4))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(9), 2)
                    BREAK_SWITCH
                END_IF
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(11), LW(12), LW(13))
                IF_INT_GREATER_OR_EQUAL(LW(10), LW(11))
                    BREAK_SWITCH
                END_IF
                CALL_CHILD(damage_check_no_last)
                SET_VALUE(LW(9), 2)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_HIT, LW(0))
                CASE_END
                CASE_EQUAL(2)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2, LW(3))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_PART, LW(4))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(9), 3)
                    BREAK_SWITCH
                END_IF
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(11), LW(12), LW(13))
                IF_INT_LESS(LW(10), LW(11))
                    BREAK_SWITCH
                END_IF
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_HIT, LW(0))
                IF_INT_EQUAL(LW(14), 0)
                    CALL_CHILD(damage_check_no_last)
                ELSE
                    CALL_CHILD(damage_check_last)
                END_IF
                SET_VALUE(LW(9), 3)
                CASE_END
                CASE_EQUAL(3)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1, LW(3))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_PART, LW(4))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(9), 4)
                    BREAK_SWITCH
                END_IF
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(11), LW(12), LW(13))
                IF_INT_LESS(LW(10), LW(11))
                    BREAK_SWITCH
                END_IF
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_HIT, LW(0))
                IF_INT_EQUAL(LW(14), 0)
                    CALL_CHILD(damage_check_no_last)
                ELSE
                    CALL_CHILD(damage_check_last)
                END_IF
                SET_VALUE(LW(9), 4)
                CASE_END
                CASE_EQUAL(4)
                BREAK_LOOP
                CASE_END
            END_SWITCH
            LABEL(10)
        END_LOOP
    END_SIBLING_EVENT
    // The boomerang spins in the air.
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 64)
        END_LOOP
    END_SIBLING_EVENT
    // It flies out to the edge of the stage and curves back to the thrower.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK2"), EVT_NULLPTR, 0, LW(8))
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), -200, LW(1), LW(2), 40, FLOAT(-20.0f), EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK3"), EVT_NULLPTR, 0, LW(8))
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2), 40, FLOAT(20.0f), EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(8))
    // It disappears into the Boomerang Bro's hand and the thrown count goes up.
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kNoShadow)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_RETURNED, 1)
    RETURN
EVT_END()

/**
 * Normal attack of the Boomerang Bro: picks the target set, pays the weapon cost,
 * collects every selected enemy into unit work (slots from 1 and 3 upward),
 * plays the throw animation "BRO_A_2A" and spawns boomerang_move_event once,
 * waiting until it reports the boomerang is back (unit work 0).
 * A confused Bro aimed at its own side runs normal_attack_event_confuse instead.
 */
static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_boomerang_bros_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_boomerang_bros_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event_confuse))
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_boomerang_bros_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_boomerang_bros_attack))
    SET_VALUE(LW(0), 1)
    SET_VALUE(LW(1), 3)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_RETURNED, 0)
    LABEL(10)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), LW(3))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(1), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    ADD(LW(0), 1)
    ADD(LW(1), 1)
    GOTO(10)
    LABEL(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
    WAIT_FRAMES(27)
    SET_VALUE(LW(15), 2)
    SET_VALUE(LW(14), 1)
    SET_VALUE(LW(6), PTR(&weapon_boomerang_bros_attack))
    SPAWN_EVENT(PTR(boomerang_move_event))
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RETURNED, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_S_1"))
    WAIT_MS(500)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Two-throw attack of the Boomerang Bro (weapon_boomerang_bros_renzoku_attack):
 * selects its targets like normal_attack_event, then throws the boomerang twice, 750 ms apart,
 * spawning boomerang_move_event each time and waiting until both have returned (unit work 0 reaches 2).
 * Aimed at its own side (confusion) it runs normal_attack_event_confuse.
 */
static EVT_BEGIN(renzoku_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_boomerang_bros_renzoku_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_boomerang_bros_renzoku_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event_confuse))
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_boomerang_bros_renzoku_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_boomerang_bros_renzoku_attack))
    SET_VALUE(LW(0), 1)
    SET_VALUE(LW(1), 3)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET1_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET2_HIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_RETURNED, 0)
    LABEL(10)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), LW(3))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(1), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    ADD(LW(0), 1)
    ADD(LW(1), 1)
    GOTO(10)
    LABEL(20)
    SET_VALUE(LW(6), PTR(&weapon_boomerang_bros_renzoku_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
    WAIT_FRAMES(27)
    SET_VALUE(LW(15), 2)
    SET_VALUE(LW(14), 0)
    SPAWN_EVENT(PTR(boomerang_move_event))
    WAIT_MS(750)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
    WAIT_FRAMES(27)
    SET_VALUE(LW(15), 4)
    SET_VALUE(LW(14), 1)
    SPAWN_EVENT(PTR(boomerang_move_event))
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RETURNED, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 2)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_S_1"))
    WAIT_MS(500)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Attack of a Boomerang Bro whose chosen target is on its own side (confusion),
 * run from normal_attack_event and renzoku_attack_event:
 * it turns around and throws the boomerang (part 2) out to x 250 and back to its home position,
 * without the per-target tracking of boomerang_move_event.
 * Then for each selected target a dodge plays the matching avoid reaction,
 * and a hit gets two damage checks 30 to 39 frames apart (the guard result is taken once, unit work 7).
 */
EVT_BEGIN(normal_attack_event_confuse)
    // The attack is announced and the guard timing is awaited, with the targets being those of its own side.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_boomerang_bros_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_boomerang_bros_attack))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    // The Boomerang Bro turns around and winds up.
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
    WAIT_FRAMES(27)
    SET_VALUE(LW(15), 2)
    // The boomerang spins out from its hand across the stage and curves back to its home spot.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, -180)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(15), 0, 0, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kNoShadow)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 64)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK2"), EVT_NULLPTR, 0, LW(8))
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), 250, 40, 0, 15, FLOAT(-20.0f), EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), -30, 40, 0, 15, FLOAT(20.0f), EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK3"), EVT_NULLPTR, 0, LW(8))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2), 15, FLOAT(-10.0f), EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kNoShadow)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // Each target is checked in turn.
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_boomerang_bros_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // A target that dodges plays its dodge reaction.
    LABEL(90)
    GOTO(97)
    // A target that is hit takes the damage twice, a moment apart, from the boomerang's out and back passes.
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, LW(8))
        IF_INT_EQUAL(LW(8), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_boomerang_bros_attack))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, 1)
        END_IF
        ADD(LW(10), 1)
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_boomerang_bros_attack), 0, LW(5))
        CALL(evt_sub_random, 10, LW(0))
        ADD(LW(0), 30)
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_boomerang_bros_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SIBLING_EVENT
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(0)
    END_IF
    // After the last target the boomerang is done and the Bro waits for the effects to settle.
    LABEL(99)
    WAIT_FRAMES(100)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/**
 * Confused version of the two-throw attack: throws the boomerang out to x 250 and back twice,
 * then for each selected target a dodge plays the avoid reaction
 * and a hit gets four damage checks 30 to 39 frames apart.
 * Nothing in this unit runs it; renzoku_attack_event uses normal_attack_event_confuse instead.
 */
EVT_BEGIN(renzoku_attack_event_confuse)
    // The attack is announced and the guard timing is awaited, with the targets being those of its own side.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_boomerang_bros_renzoku_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_boomerang_bros_renzoku_attack))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    // The Boomerang Bro turns around and throws the boomerang out across the stage and back, twice in a row.
    LOOP(2)
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BRO_A_2A"))
        WAIT_FRAMES(27)
        SET_VALUE(LW(15), 2)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("BRO_A_1B"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, -180)
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(15), 0, 0, 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kNoShadow)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(15), 0, 0, 64)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK2"), EVT_NULLPTR, 0, LW(8))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), 250, 40, 0, 15, FLOAT(-20.0f), EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), -30, 40, 0, 15, FLOAT(20.0f), EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(evt_snd_sfxoff, LW(8))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK3"), EVT_NULLPTR, 0, LW(8))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2), 15, FLOAT(-10.0f), EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(evt_snd_sfxoff, LW(8))
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kNoShadow)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOOME_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_LOOP
    // Each target is checked in turn.
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_boomerang_bros_renzoku_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // A target that dodges plays its dodge reaction.
    LABEL(90)
    GOTO(97)
    // A target that is hit takes the damage four times, a moment apart, as the boomerang whips past.
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, LW(8))
        IF_INT_EQUAL(LW(8), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_boomerang_bros_renzoku_attack))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_DEFENCE_DONE, 1)
        END_IF
        ADD(LW(10), 1)
    END_IF
    BEGIN_SIBLING_EVENT
        LOOP(3)
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_boomerang_bros_renzoku_attack), 0, LW(5))
            CALL(evt_sub_random, 10, LW(0))
            ADD(LW(0), 30)
            WAIT_FRAMES(LW(0))
        END_LOOP
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_boomerang_bros_renzoku_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SIBLING_EVENT
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(0)
    END_IF
    // After the last target the Bro waits for the effects to settle.
    LABEL(99)
    WAIT_FRAMES(160)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/** Wait event: sets the idle animation from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

