#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "system.h"
#include "unit_boss_cortez.h"

extern EvtWord cortez_weapon_blow_dead_event[];
extern EvtWord cortez_weapon_break_event[];
extern EvtWord cortez_weapon_counter_damage_event[];
extern EvtWord cortez_weapon_dead_event[];
extern EvtWord cortez_weapon_phase_event[];
extern EvtWord cortez_weapon_unison_phase_event[];

BattleUnitKind unit_boss_cortez_claw;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
BattleWeapon weapon_cortez_claw_float_attack;
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table[4];
#else
static PoseTableEntry pose_table[6];
#endif
static BattleDataEntry data_table[18];
static BattleUnitKindPart parts[1];
static EvtWord init_event[47];
static EvtWord damage_event[10];
static EvtWord attack_event[353];
static EvtWord wait_event[10];
static EvtWord cortez_damage_sub_event[25];

/**
 * Unit data of Cortez Hook: 4 HP, 65x40 units in size; ties together its sounds, parts,
 * status vulnerability, init event and event table.
 */
BattleUnitKind unit_boss_cortez_claw = {
    UNIT_CORTEZ_HOOK, // unitType
    "btl_un_cortez_claw", // name
    4, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    0, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    20, // pbSoftCap
    65, // width
    40, // height
    {0, 30}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {33.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {32.5f, 0.0f, 0.0f}, // bintaHitOffset
    {32.5f, 26.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    65.0f, // cutWidth
    40.0f, // cutHeight
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
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of Cortez Hook: 0 against every kind of hit. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of Cortez Hook: none, every element hits it normally. */
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

/**
 * Weapon of Cortez Hook (float attack): targets one unit for 2 damage, may inflict Poison (50%, 10 turns);
 * used by attack_event.
 */
BattleWeapon weapon_cortez_claw_float_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    50, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseTableEntry pose_table[] = {
#if VERSION <= VERSION_DEMO_US
    {28, "KRT_S_1"},
#else
    {POSE_TYPE_STOP, "KRT_Z_1"},
    {POSE_TYPE_FREEZE, "KRT_Z_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
#endif
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_S_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

/**
 * Event table of Cortez Hook: cortez_damage_sub_event (tag 9), cortez_weapon_dead_event (tag 49),
 * cortez_weapon_break_event (tags 70, 71), cortez_weapon_blow_dead_event (tag 58),
 * cortez_weapon_counter_damage_event (tags 36, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, cortez_damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, cortez_weapon_dead_event},
#if VERSION <= VERSION_DEMO_US
    {70, cortez_weapon_break_event},
#else
    {UNIT_DATA_WEAPON_BREAK_EVENT, cortez_weapon_break_event},
#endif
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
    {0, NULL},
};

/** Parts of Cortez Hook: 1 part (model c_buki4). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_cortez_claw", "c_buki4", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kHidden | kNoShadow, 0, pose_table},
};

/**
 * Init event of Cortez's claw weapon: sets the unit flag 0x1000000, installs its wait, phase, unison phase,
 * attack, damage and (default) confusion events,
 * stores weapon number 2 in unit work 2 and clears unit work 1 (the flag that makes it sit out its attack),
 * and allows no movement.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(cortez_weapon_phase_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(cortez_weapon_unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, CORTEZ_WEAPON_WORK_INDEX, 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, CORTEZ_WEAPON_WORK_DOWN_TURNS, 0)
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of Cortez's claw: runs the default damage handling with LW(10) = -2 and LW(11) = 1. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Attack event of Cortez's claw; while knocked down (unit work 1, its revive countdown, is not 0) it only
 * restarts the wait event.
 * Otherwise it picks a target with weapon_cortez_claw_float_attack,
 * winds back to -135 degrees in z as it dives in front of the target,
 * then swings forward to 45 degrees with the sword-dance sound for one hit.
 * It plays the avoid pose on a dodge, or applies the damage with the action command defence check;
 * a superguarded claw recoils home at once, otherwise it straightens and returns home.
 */
static EVT_BEGIN(attack_event)
    // While the Hook is knocked down it only goes back to idling.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, CORTEZ_WEAPON_WORK_DOWN_TURNS, LW(0))
    // Any nonzero down-turn count suppresses this attack.
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&weapon_cortez_claw_float_attack))
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
    // The Hook leans back to -135 degrees while it dives in front of its target.
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 25, 12, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(20)
        CALL(evt_sub_intpl_init, EASING_CUBIC_IN, 0, -135, 40)
        LOOP(41)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(200)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init, EASING_LINEAR, -135, 45, 10)
        LOOP(11)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    // It swings forward to 45 degrees in a single slash, with a whooshing sound.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_DANCE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(7)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // The swing misses or is blocked: the Hook plays its dodge reaction.
    LABEL(90)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    GOTO(95)
    // The swing lands: the Action Command defence result and damage are applied.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    // A superguard cancels the swing event and takes a separate recoil path home.
    IF_INT_EQUAL(LW(5), 21)
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
        ADD(LW(1), 15)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, 0, -30, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 21, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, -30, 0, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        RETURN
    END_IF
    // Otherwise it straightens up and returns home after a short pause.
    LABEL(95)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_MS(500)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 45, 0, 15)
        LOOP(15)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Idle event of Cortez's claw: waits for the loop timing of its animation
 * and plays the pose table pose for state 1.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeWaitPlayLoopTiming, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Data event (entry 9) of Cortez's claw: when the weapon takes damage it plays the KRT_Z_1 pose on the unit in
 * LW(10) and waits 30 frames.
 */
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

