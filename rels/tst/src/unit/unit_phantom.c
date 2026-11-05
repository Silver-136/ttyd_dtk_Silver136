// Battle unit Phantom Ember (`phantom`): tst.rel's copy of rels/jon/src/unit/unit_phantom.c (parts, stats, poses and AI scripts).

#include "version.h"
#include "battle/battle_unit.h"
#include "eff/eff_fire.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

static EffectEntry* eff;
static EffectEntry* eff2[10];
static s32 step[10];

static USER_FUNC(eff_all_fire);
static USER_FUNC(eff_fire);
static USER_FUNC(bubble_copy_status);

BattleUnitKind unit_phantom;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[4];
static BattleUnitKindPart parts[2];
static BattleWeapon weapon_bubble_attack;
static BattleWeapon weapon_bubble_fire_attack;
static BattleUnitSetup entry_bubble;
static EvtWord init_event[39];
static EvtWord damage_event[10];
static EvtWord avoid_fire_event[16];
static EvtWord damage_fire_explosion_event[103];
static EvtWord friend_event[99];
static EvtWord attack_event[51];
static EvtWord normal_attack_event[412];
static EvtWord fire_attack_event[365];
static EvtWord all_fire_attackEvent_sub[10];
static EvtWord all_fire_attack_event[321];
static EvtWord wait_event[6];

/**
 * Script helper: args[0] = source unit, args[1] = destination unit.
 * Copies the current HP (clamped to the destination's maximum)
 * and the size-change status from one Phantom Ember to another, and resets the destination's HP gauge to match.
 * The Phantom Ember's friend_event calls it right after spawning a second Phantom Ember (the split),
 * so the new one starts with the same HP.
 */
static USER_FUNC(bubble_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->currentHp = unit1->currentHp;
    if (unit2->currentHp > unit2->maxHp) {
        unit2->currentHp = unit2->maxHp;
    }
    unit2->sizeChangeTurns = unit1->sizeChangeTurns;
    unit2->sizeChangeStrength = unit1->sizeChangeStrength;
    unit2->hpGauge.prevHp = unit2->currentHp;
    unit2->hpGauge.targetHp = unit2->currentHp;
    unit2->hpGauge.barRatio = unit2->hpGauge.trailRatio = (f32)unit2->currentHp / unit2->maxHp;
    return EVT_RETURN_DONE;
}

/** Unit data of the Phantom Ember: stats, sounds, attributes, parts, init script and data table. */
BattleUnitKind unit_phantom = {
    UNIT_PHANTOM_EMBER, // unitType
    "btl_un_phantom", // name
    10, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    30, // level
    0, // bonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    26, // width
    36, // height
    {8, 24}, // hitOffset
    {0.0f, 18.0f, 0.0f}, // centerOffset
    {0, -3}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {13.0f, 23.4f, 0.0f}, // kissHitOffset
    {0.0f, 18.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    36.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_BUBBLE_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Damage reduction of the Phantom Ember: none, every attack type does full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/**
 * Element response of the Phantom Ember: it takes extra damage from ice and explosions and is healed by
 * fire.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_ABSORB, DEFENSE_ATTR_WEAK, DEFENSE_ATTR_WEAK, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {40, 80, 65, 25, 65, 100, 0, 40, 100, 65, 100, 65, 100, 65, 65, 40, 80, 100, 65, 100, 100, 90};
#else
static StatusVulnerability regist = {
    40, // sleep
    80, // stop
    100, // dizzy
    100, // poison
    65, // confuse
    100, // electric
    0, // burn
    40, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    90, // defenseDown
    65, // allergic
    40, // fright
    80, // galeForce
    100, // fast
    65, // slow
    100, // dodgy
    100, // invisible
    90, // ohko
};
#endif

/** Pose table of the Phantom Ember: battle pose types mapped to animation names. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "A_2B"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/**
 * Data table of the Phantom Ember: event handlers keyed by tag (damage_fire_explosion_event,
 * avoid_fire_event).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_FIRE_EXPLOSION_EVENT, damage_fire_explosion_event},
    {UNIT_DATA_AVOID_FIRE_EVENT, avoid_fire_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Body parts of the Phantom Ember: hit position, size, defence tables and attributes. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_phantom", "c_phantom", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 35.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kWeakToIcePower, kCounterFiery, pose_table},
    {2, NULL, "c_phantom", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/** Attack data for the Phantom Ember's burst of flame on contact: 5 damage. */
static BattleWeapon weapon_bubble_attack = {
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
#endif
    COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Attack data for the Phantom Ember's fireball: 5 damage, 75% Burn. */
static BattleWeapon weapon_bubble_fire_attack = {
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    75, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Spawn setup for the extra Phantom Ember created when it splits: enemy side, parked off-screen until its
 * script places it.
 */
static BattleUnitSetup entry_bubble = {
    &unit_phantom, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Init event of the Phantom Ember: installs its wait, attack,
 * damage and confusion events and lifts it to height 10 (both position and home position)
 * so it floats above the stage.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage event of the Phantom Ember: runs the default damage handling with LW(10) = -2
 * and LW(11) = 1 as its parameters.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Plays the flame sound and the 2B pose for one second during the Phantom Ember's fiery-contact counter reaction. */
static EVT_BEGIN(avoid_fire_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    WAIT_MS(1000)
    RETURN
EVT_END()

/**
 * Data table tag 11: btldefaultevt_Damage runs it instead of the usual reaction
 * when a fire hit (damage code 0x18)
 * lands on the Phantom Ember.
 * It scans in both directions from the unit's position for a free spot (btlevtcmd_CheckSpace) and,
 * on finding one, calls friend_event to split off a new flame there.
 */
static EVT_BEGIN(damage_fire_explosion_event)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    // BUG: LW(0) is the unit's x coordinate from GetPos, so this waits that many frames
    // (a negative x would not count down to 0).
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        IF_INT_LESS_OR_EQUAL(LW(0), 170)
            ADD(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(friend_event)
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        IF_INT_GREATER_OR_EQUAL(LW(0), 10)
            SUBTRACT(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(friend_event)
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    LABEL(99)
    RETURN
EVT_END()

/**
 * Splits a new Phantom Ember off the current one:
 * spawns entry_bubble at its position, copies HP and status to it (bubble_copy_status),
 * plays the split sound and makes the new flame spin
 * and hop to the free spot found by the caller (LW(0) to LW(2)), which becomes its home position.
 */
static EVT_BEGIN(friend_event)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(12), LW(13), LW(14))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_bubble), 0)
    CALL(bubble_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetPos, LW(3), LW(12), LW(13), LW(14))
    CALL(btlevtcmd_JumpSetting, LW(3), 0, 10, FLOAT(0.5f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 10, 0)
        SET_VALUE(LW(12), 0)
        LOOP(0)
            WAIT_FRAMES(1)
            ADD(LW(12), 1)
            IF_INT_GREATER_OR_EQUAL(LW(12), 30)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddRotate, LW(3), 0, 0, 40)
        END_LOOP
        CALL(btlevtcmd_SetRotate, LW(3), 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), LW(1), LW(2), FLOAT(30.0f), MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    WAIT_MS(1000)
    RETURN
EVT_END()

/**
 * Attack event of the Phantom Ember: runs a selected enemy-item script if one is returned,
 * otherwise rolls 0 to 99 and does the bubble attack (normal_attack_event) below 70,
 * else the fire attack (fire_attack_event).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        GOTO(99)
        RETURN
    END_IF
    SET_VALUE(LW(0), 70)
    ADD(LW(0), 30)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 70)
        SET_VALUE(LW(9), PTR(&weapon_bubble_attack))
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_bubble_fire_attack))
    CALL_CHILD(fire_attack_event)
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Phantom Ember's plain attack: it swoops at the chosen target in two dives with the move sound.
 * If the target avoids it, it plays the flame sound and the 1B pose;
 * on a hit it shakes the camera, applies the damage with the action command defence check,
 * then swoops back and returns to its idle pose.
 */
static EVT_BEGIN(normal_attack_event)

    // Pick the target (the First Strike target or a sampled enemy); a confused Phantom
    // Ember with nobody to hit gets flustered.
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
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)

    // The Phantom Ember floats over to the target with its flames flickering.
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(120)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 13)
    SUBTRACT(LW(1), 20)
    IF_INT_LESS(LW(1), 0)
        SET_VALUE(LW(1), 0)
    END_IF
    ADD(LW(2), 5)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 5)
    SUBTRACT(LW(1), 20)
    IF_INT_LESS(LW(1), 0)
        SET_VALUE(LW(1), 0)
    END_IF
    ADD(LW(2), 5)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    DELETE_EVENT(LW(15))
    WAIT_FRAMES(30)

    // Find out whether the burst reaches the target.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // Miss or dodge: it still bursts into flame four times in front of the target.
    LABEL(90)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(4)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
        WAIT_FRAMES(10)
    END_LOOP
    GOTO(98)

    // Hit: a flame burst deals the damage, followed by a few more pulses.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(10)
    LOOP(3)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
        WAIT_FRAMES(10)
    END_LOOP
    GOTO(98)

    // It hops back away from the target in two arcs, then floats to its home position.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    ADD(LW(1), 20)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 20, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 100)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -30, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 25, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(400)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Script helper: args[0] = unit.
 * Same as the Lava Bubble's flame effect (unit_bubble.c) but with effect type 7 for the Phantom Ember,
 * kept on part 2 and growing to the unit's size; blocks forever.
 */
static USER_FUNC(eff_fire) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 2);
    EffFireWork* wk;
    f32 x;
    f32 y;
    f32 z;

    if (isFirstCall) {
#if VERSION <= VERSION_DEMO_JP
        eff = effFireEntry(7, 90, 0.0f, -1000.0f, 0.0f, 1.0f);
#else
        eff = effFireEntry(7, 106, 0.0f, -1000.0f, 0.0f, 1.0f);
#endif
        ((EffFireWork*)eff->userdata)->scaleX = 0.0f;
        ((EffFireWork*)eff->userdata)->scaleY = 0.0f;
    }
    wk = eff->userdata;
    if (wk != NULL) {
        BtlUnit_GetPartsPos(part, &x, &y, &z);
        wk->position = (Vec){x, y, z};
        wk->scaleX += 0.1f * unit->sizeMultiplier;
        if (wk->scaleX >= unit->sizeMultiplier) {
            wk->scaleX = unit->sizeMultiplier;
        }
        wk->scaleY += 0.1f * unit->sizeMultiplier;
        if (wk->scaleY >= unit->sizeMultiplier) {
            wk->scaleY = unit->sizeMultiplier;
        }
    }
    return EVT_RETURN_BLOCK;
}

/**
 * The Phantom Ember's fire attack: it spits a fireball (part 2, drawn by eff_fire) that flies to the target in
 * three dives with the attack sounds.
 * On a hit it makes a smoke puff and applies the damage with the action command defence check.
 */
static EVT_BEGIN(fire_attack_event)

    // Pick the target (the First Strike target or a sampled enemy); a confused Phantom
    // Ember with nobody to hit gets flustered.
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
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // The Phantom Ember mutters to itself, then detaches its fire from its body.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("T_1"))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    END_SIBLING_EVENT

    // It throws the fireball toward the target along a curved path.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(eff_fire, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 0)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    SUBTRACT_FLOAT(LW(10), LW(0))
    SUBTRACT_FLOAT(LW(12), LW(2))
    MULTIPLY_FLOAT(LW(10), FLOAT(0.69921875f))
    MULTIPLY_FLOAT(LW(12), FLOAT(0.5996094f))
    ADD_FLOAT(LW(0), LW(10))
    ADD_FLOAT(LW(2), LW(12))
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 40, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    SUBTRACT_FLOAT(LW(10), LW(0))
    SUBTRACT_FLOAT(LW(12), LW(2))
    MULTIPLY_FLOAT(LW(10), FLOAT(0.39941406f))
    MULTIPLY_FLOAT(LW(12), FLOAT(0.39941406f))
    ADD_FLOAT(LW(0), LW(10))
    ADD_FLOAT(LW(2), LW(12))
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 20, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)

    // Find out whether the fireball hits.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(98)

    // Hit: the fireball bursts into smoke on the target and deals the burn damage.
    LABEL(91)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 16, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(400)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

static USER_FUNC(eff_all_fire) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 index = evtGetValue(event, args[1]);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[10] = {1.0f, 1.25f, 1.5f, 1.75f, 2.0f, 2.25f, 2.0f, 1.75f, 1.5f, 1.25f};
    EffFireWork* wk;
    f32 x;
    f32 y;
    f32 z;

    if (isFirstCall) {
        eff2[index] = effFireEntry(7, 120, 0.0f, -1000.0f, 0.0f, 1.0f);
        ((EffFireWork*)eff2[index]->userdata)->scaleX = 2.0f;
        ((EffFireWork*)eff2[index]->userdata)->scaleY = unit->sizeMultiplier * scales[index];
    }
    wk = eff2[index]->userdata;
    if (wk != NULL) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        wk->position = (Vec){x + 24.0f * unit->faceDirection * (index + 1), y - 15.0f - 2.0f * scales[index], z - 10.0f};
    }
    return EVT_RETURN_BLOCK;
}

/** Helper of all_fire_attack_event: starts one eff_all_fire effect for index LW(0) and waits 120 frames. */
static EVT_BEGIN(all_fire_attackEvent_sub)
    BEGIN_SIBLING_EVENT
        CALL(eff_all_fire, UNIT_ID_SELF, LW(0))
    END_SIBLING_EVENT
    WAIT_FRAMES(120)
    RETURN
EVT_END()

/**
 * Fire attack on every target: the Phantom Ember moves to x = -40,
 * shakes for 20 beats, then ten fire effects are spawned (one every 2 frames),
 * then targets are checked in turn until one misses, for avoid or damage (the attack defence result is taken once).
 * Afterwards it returns home. The unit's attack AI never selects it.
 */
static EVT_BEGIN(all_fire_attack_event)

    // Pick the target (the First Strike target or a sampled enemy); a confused Phantom
    // Ember with nobody to hit gets flustered.
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
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack on the whole party.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -40, LW(1), -15, 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
#else
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -40, LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
#endif
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // It shakes in place, then a row of ten flames springs up.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2A"))
    LOOP(20)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 2, 0, 0)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, -2, 0, 0)
        WAIT_FRAMES(2)
    END_LOOP
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(10)
            SPAWN_EVENT(all_fire_attackEvent_sub)
            WAIT_FRAMES(2)
            ADD(LW(0), 1)
        END_LOOP
    END_SIBLING_EVENT

    // The flames sweep over the party.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)

    // Check party members in turn for avoidance and damage, stopping when one misses.
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(98)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF

    // It returns home.
    LABEL(98)
    WAIT_FRAMES(180)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BUBBLE_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(400)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle event: shows the unit's table pose for part 1. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

