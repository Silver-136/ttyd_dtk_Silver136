// Battle unit Boo (`teresa`): tst.rel's copy of rels/jin/src/unit/unit_teresa.c.
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
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_sub.h"

// Unit work slots of the Boo (btlevtcmd_Get/SetUnitWork).
enum {
    UNIT_WORK_RAISED = 0, // 1 once move_event_sub has lifted it; Boos still at 0 join the next group move
};

static USER_FUNC(teresa_check_move);
static USER_FUNC(teresa_check_teresa);
static USER_FUNC(teresa_check_trans);

BattleUnitKind unit_teresa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_teresa_attack;
static BattleWeapon weapon_teresa_trans_myself;
static BattleWeapon weapon_teresa_trans;
static BattleWeapon weapon_teresa_move;
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[1];
static EvtWord init_event[47];
static EvtWord damage_event[10];
static EvtWord attack_event[153];
static EvtWord normal_attack_event[354];
static EvtWord trans_myself_event[196];
static EvtWord trans_event[289];
static EvtWord move_event[84];
static EvtWord move_event_sub[103];
static EvtWord wait_event[6];

/**
 * Script helper: args[0] is a unit id; args[1] receives 1 when that unit is a Boo
 * or Dark Boo (kinds 0x4B and 0x6A), else 0.
 */
static USER_FUNC(teresa_check_teresa) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 result = 0;
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (unit == NULL) {
        evtSetValue(event, args[1], 0);
        return EVT_RETURN_DONE;
    }
    if (unit->currentKind == 0x4B) {
        result = 1;
    }
    if (unit->currentKind == 0x6A) {
        result = 1;
    }
    evtSetValue(event, args[1], result);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] receives 1 when at least one Boo
 * or Dark Boo (kinds 0x4B, 0x6A) is still free to act (its work[0] is 0), else 0.
 */
static USER_FUNC(teresa_check_move) {
    EvtWord* args = event->args;
    s32 num = 0;
    s32 i;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit != NULL) {
            if (unit->currentKind == 0x4B && unit->work[0] == 0) {
                num++;
            }
            if (unit->currentKind == 0x6A && unit->work[0] == 0) {
                num++;
            }
        }
    }
    if (num >= 1) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is the Boo itself; args[1] receives 1 when some other Boo
 * or Dark Boo (kinds 0x4B, 0x6A) is not invisible (STATUS_INVISIBLE),
 * i.e. there is another one that can still turn visible or invisible, else 0.
 */
static USER_FUNC(teresa_check_trans) {
    EvtWord* args = event->args;
    s32 num = 0;
    s32 id = evtGetValue(event, args[0]);
    s32 self;
    s32 i;

    self = BattleTransID(event, id);

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit != NULL) {
            if (unit->currentKind == 0x4B && !BtlUnit_CheckStatus(unit, 0x12) && i != self) {
                num++;
            }
            if (unit->currentKind == 0x6A && !BtlUnit_CheckStatus(unit, 0x12) && i != self) {
                num++;
            }
        }
    }
    if (num >= 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Unit definition of the Boo: 7 HP and level 17, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_teresa = {
    75,
    "btl_un_teresa",
    7,
    0,
    1,
    1,
    17,
    0,
    2,
    70,
    1,
    50,
    9999,
    34,
    32,
    0,
    32,
    0.0f,
    16.0f,
    0.0f,
    0,
    -3,
    0.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    0.0f,
    17.0f,
    20.8f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    34.0f,
    32.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_TELESA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    UNIT_ATTR_FLOATING,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/** Defence of the Boo: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the Boo: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
/** Status vulnerability of the Boo in the demo builds: immune to poison; resists most other statuses. */
static StatusVulnerability regist = {50, 50, 90, 0, 50, 100, 100, 50, 100, 60, 100, 60, 100, 60, 60, 50, 95, 100, 10, 100, 100, 10};
#else
/**
 * Status vulnerability of the Boo in the retail builds: immune to poison; extra prone to dizzy;
 * resists most other statuses.
 */
static StatusVulnerability regist = {50, 50, 105, 0, 50, 100, 100, 50, 100, 90, 100, 90, 100, 95, 60, 50, 90, 100, 10, 100, 100, 10};
#endif

/** Body part of the Boo: a single part using the c_teresa model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_teresa", "c_teresa", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/** Attack data of the Boo's attack: 3 damage. */
static BattleWeapon weapon_teresa_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Attack data of the Boo's spell that turns itself invisible: no direct damage;
 * 100% chance of invisible for 2 turns.
 */
static BattleWeapon weapon_teresa_trans_myself = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, // holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Attack data of the Boo's spell that turns an ally invisible: no direct damage; aimed at its own side;
 * 100% chance of invisible for 2 turns.
 */
static BattleWeapon weapon_teresa_trans = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Attack data of the Boo's support move aimed at an ally: no direct damage; aimed at its own side. */
static BattleWeapon weapon_teresa_move = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Pose table of the Boo: maps each battle pose (waiting, hurt, running...) to an animation. */
static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "X_1"},
    {BTL_POSE_NORMAL, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "A_2"},
    {BTL_POSE_DAMAGE, "D_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Data table of the Boo: it has no special reaction scripts. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Registers the Boo's wait, attack and damage events (confusion reuses the attack event),
 * clears unit work 0 (set once it has risen out of reach in move_event),
 * puts it 15 units above the ground and starts the idle loop.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_RAISED, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 15)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Hit reaction: just the default btldefaultevt_Damage for its single part. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * The Boo's turn (also used when confused): on a first strike it attacks;
 * otherwise uses a held item if the AI wants to, then rolls 0-114:
 * under 25 it turns itself invisible if it is not already, under 50 it turns another visible Boo invisible,
 * under 65 it lifts the Boos into the air if one is still grounded.
 * A roll whose move cannot be done falls through to the next check, and finally to the attack.
 * While confused it never turns another Boo invisible or lifts the Boos.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INVISIBLE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(13), 0)
    ELSE
        SET_VALUE(LW(13), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 0)
    ELSE
        CALL(teresa_check_trans, UNIT_ID_SELF, LW(14))
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(15), 0)
    ELSE
        CALL(teresa_check_move, LW(15))
    END_IF
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 50)
    ADD(LW(0), 25)
    ADD(LW(0), 25)
    ADD(LW(0), 15)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 25)
    IF_INT_LESS(LW(1), LW(0))
        IF_INT_EQUAL(LW(13), 1)
            CALL_CHILD(trans_myself_event)
            GOTO(99)
        END_IF
    END_IF
    ADD(LW(0), 25)
    IF_INT_LESS(LW(1), LW(0))
        IF_INT_EQUAL(LW(14), 1)
            CALL_CHILD(trans_event)
            GOTO(99)
        END_IF
    END_IF
    ADD(LW(0), 15)
    IF_INT_LESS(LW(1), LW(0))
        IF_INT_EQUAL(LW(15), 1)
            CALL_CHILD(move_event)
            GOTO(99)
        END_IF
    END_IF
    CALL_CHILD(normal_attack_event)
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Basic attack: fades out, reappears beside the target and lunges for 3 damage (weapon_teresa_attack).
 * On a miss or dodge it holds its surprised pose; then it turns around and floats home.
 */
static EVT_BEGIN(normal_attack_event)
    // First-strike battle: the target is already fixed; otherwise one is picked from the opposing party.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_teresa_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_attack), LW(3), LW(4))
    END_IF

    // No target to attack: a confused Boo just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_teresa_attack))

    // The camera frames the Boo and its target.
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)

    // It fades out, drifts toward the target and fades back in beside it.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
        SET_VALUE(LW(7), 256)
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
        END_LOOP
        WAIT_FRAMES(20)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
        SET_VALUE(LW(7), -1)
        LOOP(8)
            WAIT_FRAMES(1)
            ADD(LW(7), 32)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 15)
    ADD(LW(2), 5)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -5, 4, 0, -1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    WAIT_FRAMES(15)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(9)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT

    // It lunges at the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_FRAMES(15)

    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_attack), 0, LW(5))
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
        // A miss: the Boo shies away from the target (pose X_1).
        LABEL(90)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("X_1"))
        WAIT_FRAMES(60)
        GOTO(98)

        // A hit: the camera shakes and the target takes the damage.
        LABEL(91)
        CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_add_zoom, 0, -50)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        WAIT_FRAMES(30)
        GOTO(98)

        // It floats back to its place.
        LABEL(98)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)

        // It turns away from the target.
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * Makes the Boo itself invisible for 2 turns (weapon_teresa_trans_myself, always hits):
 * it backs off, fades out unless already invisible, spins in place and comes back.
 */
static EVT_BEGIN(trans_myself_event)
    SET_VALUE(LW(3), -2)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INVISIBLE, LW(8))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_teresa_trans_myself))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 10, 4, 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    IF_INT_EQUAL(LW(8), 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
        SET_VALUE(LW(7), 256)
        BEGIN_SIBLING_EVENT
            LOOP(16)
                WAIT_FRAMES(1)
                SUBTRACT(LW(7), 8)
                CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 0, 5, 0, -1)
    WAIT_FRAMES(10)
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 15, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 15, 0)
    END_LOOP
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_trans_myself), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

/**
 * Makes another Boo invisible for 2 turns: keeps picking random allies until one is a Boo
 * or Dark Boo that is not invisible (gives up only when no target is found),
 * floats over to it fading out and in, touches it with weapon_teresa_trans_myself and floats home.
 */
static EVT_BEGIN(trans_event)
    LABEL(0)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_teresa_trans))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_trans), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(teresa_check_teresa, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INVISIBLE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(1)
        END_IF
    END_IF
    GOTO(0)
    LABEL(1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_trans))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_teresa_trans))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    ELSE
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    SET_VALUE(LW(7), 256)
    BEGIN_SIBLING_EVENT
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 40)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -5, 4, 0, -1)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
    SET_VALUE(LW(7), -1)
    LOOP(16)
        WAIT_FRAMES(1)
        ADD(LW(7), 16)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, 0, 0, -1)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_trans_myself), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -5, 4, 0, -1)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    LABEL(99)
    RETURN
EVT_END()

/** tst has the JP/US move_event, without the PAL two-teresa wait minimum */
static EVT_BEGIN(move_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_teresa_move))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_move), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(30)
    SET_VALUE(LW(12), 80)
    LABEL(0)
    CALL(teresa_check_teresa, LW(3), LW(5))
    IF_INT_EQUAL(LW(5), 1)
        CALL(btlevtcmd_GetUnitWork, LW(3), UNIT_WORK_RAISED, LW(6))
        IF_INT_EQUAL(LW(6), 0)
            SPAWN_EVENT(move_event_sub)
            ADD(LW(12), 20)
        END_IF
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(0)
    END_IF
    WAIT_FRAMES(LW(12))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    RETURN
EVT_END()

/**
 * Spawned by move_event for the Boo in LW(3): marks its unit work 0,
 * makes it untargetable by hammer and shell-toss attacks and raises it to height 40 as its new home.
 * The acting Boo goes at once in its T_1 pose; the others wait 60-80 frames and spin as they rise.
 */
static EVT_BEGIN(move_event_sub)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(1))
    IF_INT_EQUAL(LW(3), LW(1))
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(1), 0)
    ELSE
        CALL(evt_sub_random, 20, LW(0))
        ADD(LW(0), 60)
        SET_VALUE(LW(1), 12)
    END_IF
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), UNIT_WORK_RAISED, 1)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
    BEGIN_SIBLING_EVENT
        LOOP(60)
            CALL(btlevtcmd_AddRotate, LW(3), 0, LW(1), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
        IF_INT_EQUAL(LW(3), LW(0))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("T_1"))
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPartsPos, LW(3), 1, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 40)
    CALL(btlevtcmd_DivePosition, LW(3), LW(0), LW(1), LW(2), 60, 10, 4, 0, -1)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    RETURN
EVT_END()

/** Idle: plays the stand pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
