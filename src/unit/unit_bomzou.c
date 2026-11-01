// Battle unit data for the Bob-omb bomb (unit_bomzou) and its explosion weapon, used by Bobbery.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"
#include "battle/unit_work.h"


static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table_3[4];
static PoseTableEntry pose_table_2[4];
static PoseTableEntry pose_table_1[4];
BattleUnitKind unit_bomzou;
BattleWeapon weapon_bomzou_explosion;
static BattleUnitKindPart parts[1];
static EvtWord init_event[70];
static EvtWord phase_event[67];
static EvtWord wait_event[57];
EvtWord end_battle_evt[19];
#if VERSION <= VERSION_DEMO_US
static EvtWord explosion_event[172];
#else
static EvtWord explosion_event[173];
#endif
static BattleDataEntry data_table[4];
static EvtWord damage_event[10];
static EvtWord attack_event[5];
static EvtWord dead_event[6];

/** Damage reduction of the bomb: none. It has 1 HP, so any hit knocks it out (and sets it off). */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** The bomb has no elemental defence. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** The bomb has no status vulnerabilities (all zero). */
static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    0, // electric
    0, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

/** Poses while the fuse has 3 or more turns left (all frames use B_2). */
static PoseTableEntry pose_table_3[] = {
    {POSE_TYPE_NORMAL, "B_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "B_2"},
    {POSE_TYPE_DAMAGE, "B_2"},
    {POSE_TYPE_ACTION, "B_2"},
};

/** Poses while the fuse has 2 turns left (all frames use B_3). */
static PoseTableEntry pose_table_2[] = {
    {POSE_TYPE_NORMAL, "B_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "B_3"},
    {POSE_TYPE_DAMAGE, "B_3"},
    {POSE_TYPE_ACTION, "B_3"},
};

/** Poses while the fuse has 1 turn left (all frames use B_4). */
static PoseTableEntry pose_table_1[] = {
    {POSE_TYPE_NORMAL, "B_4"},
    {POSE_TYPE_DAMAGE_GUARDED, "B_4"},
    {POSE_TYPE_DAMAGE, "B_4"},
    {POSE_TYPE_ACTION, "B_4"},
};

/**
 * Unit data of the bomb that Admiral Bobbery's Bomb Squad leaves on the field (its debug name in the game
 * text is "BOMBZO GLOBAL 1701"): 1 HP, no HP gauge or damage numbers,
 * explodes when its fuse runs out or when it is destroyed.
 */
BattleUnitKind unit_bomzou = {
    UNIT_BOMB_SQUAD_BOMB, // unitType
    "btl_un_bomzou", // name
    1, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    1, // level
    0, // bonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    24, // width
    24, // height
    {0, 20}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 0.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {12.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 15.6f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BTL_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kNoDamageNumber | kUnitNoHpGauge | kUnitNoncombatant, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * The bomb's blast: 3 damage with the explosion element to every enemy in range, ignoring guarding,
 * able to shatter frozen enemies; used by explosion_event.
 */
BattleWeapon weapon_bomzou_explosion = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_OR_SEMI_PARTS | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_EXPLOSION, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 10, 10, 100, 30, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/**
 * The bomb's single part: the bomb model (c_bakudan), the main target,
 * which cannot be tattled or have its HP shown.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_bomzou", "c_bakudan", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kUntattleable, 0, pose_table_3},
};

/**
 * Init for a bomb that Bobbery's Bomb Squad leaves on the field:
 * registers the wait, phase, attack and damage scripts,
 * sets the fuse to 2 turns (unit work 0) and clears the stored sizzle sound id (unit work 1).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_TURNS_LEFT, 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_FUSE_SFX, 0)
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_TURNS_LEFT, LW(0))
    SWITCH(LW(0))
        CASE_GREATER_OR_EQUAL(3)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_3))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_2))
        CASE_GREATER_OR_EQUAL(1)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_1))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("B_1"))
    RETURN
EVT_END()

/**
 * At the start of each player phase, burns one turn off the fuse and swaps the pose table to the shorter fuse;
 * when the fuse reaches 0 it takes the phase, plays the "B_2" pose for 36 frames and explodes.
 */
static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_TURNS_LEFT, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_TURNS_LEFT, LW(0))
        SWITCH(LW(0))
            CASE_GREATER_OR_EQUAL(3)
            CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_3))
            CASE_GREATER_OR_EQUAL(2)
            CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_2))
            CASE_GREATER_OR_EQUAL(1)
            CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_1))
        END_SWITCH
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("B_2"))
            WAIT_FRAMES(36)
            CALL_CHILD(PTR(explosion_event))
            ELSE
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Idle loop: plays the fuse pose, and once only one turn is left on the fuse puffs a sweat effect above the bomb
 * every 45-89 frames as a warning.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_TURNS_LEFT, LW(0))
    IF_INT_GREATER(LW(0), 1)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(evt_eff64, 0, STRING("sweat_n64"), 1, LW(0), LW(1), LW(2), 15, -45, 15, 0, 0, 0, 0, 0)
    CALL(evt_sub_random, 45, LW(0))
    ADD(LW(0), 45)
    WAIT_FRAMES(LW(0))
    GOTO(0)
    RETURN
EVT_END()

/**
 * Battle-end data event (tag 63, run by the battle end sequence):
 * stops the fuse sizzle loop whose sound id Bobbery's Bomb Squad throw stored in unit work 1,
 * so the sound does not outlive the battle.
 */
EVT_BEGIN(end_battle_evt)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_FUSE_SFX, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_snd_sfxoff, LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_FUSE_SFX, 0)
    END_IF
    RETURN
EVT_END()

/**
 * The blast: stops the fuse sizzle, plays the fire pillar effect, sound and screen shake, clears stage fog,
 * hides the bomb, damages every enemy within 50 units horizontally with weapon_bomzou_explosion (3 damage),
 * then removes the bomb.
 */
static EVT_BEGIN(explosion_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_FUSE_SFX, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_snd_sfxoff, LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, BOMZOU_WORK_FUSE_SFX, 0)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("hibashira"), 2, LW(0), LW(1), LW(2), LW(0), LW(1), LW(2), 1, FLOAT(1.0f), 60, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_THUNDERS_BOMB2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_StageDispellFog)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden | kNoShadow)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 4)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("B_2"))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, 1, PTR(&weapon_bomzou_explosion))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_bomzou_explosion), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), LW(3))
        GOTO(80)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), -1)
        MULTIPLY(LW(0), -1)
    END_IF
    IF_INT_GREATER(LW(0), 50)
        GOTO(80)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_bomzou_explosion), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_bomzou_explosion), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    LABEL(80)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
#if VERSION <= VERSION_DEMO_US
EVT_END()
#else
EVT_END_PAD()

/**
 * Data table events: knocked out (49) queues the blast, tag 53 is the blast itself,
 * and the battle end (63) cuts off the fuse sound.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_EXPLOSION_EVENT_BOMZOU, explosion_event},
    {UNIT_DATA_BATTLE_END_EVENT, end_battle_evt},
    {0, NULL},
};
#endif

/** Hit reaction: the default damage script with no special pose (LW10 = -2). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/** The bomb never acts on its own turn; it just returns to its idle loop. */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Death data event (tag 49): queues explosion_event (tag 53) as an after-reaction,
 * so a bomb that is knocked out still blows up.
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_AfterReactionEntry, UNIT_ID_SELF, UNIT_DATA_EXPLOSION_EVENT_BOMZOU)
    RETURN
EVT_END()

