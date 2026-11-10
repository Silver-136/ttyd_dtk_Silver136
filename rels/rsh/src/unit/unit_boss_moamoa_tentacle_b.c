// Smorg Miasma (moamoa_tentacle_b), Smorg's tentacle B part of the boss battle unit, used on the Excess Express.

#include "version.h"
#include "battle/battle_unit.h"
#include "rsh.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "unit_boss_moamoa.h"

extern PoseTableEntry moamoa1_weakness_pose_table[];
extern BattleUnitDefense moamoa_defence_no_parts;

BattleUnitKind unit_boss_moamoa_tentacle_b;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_moamoa_attack;
static EvtWord init_event[26];
static EvtWord damage_event[10];
static EvtWord dead_event[115];
static EvtWord phase_event[2];
static EvtWord attack_event[21];
static EvtWord evt_attack_other_arm[81];
const char* A_of_B_Attack[3];
const char* B_of_B_Attack_front[10];
const char* B_of_B_Attack_back[10];
const char* C_of_B_Attack[6];
static EvtWord evt_attack[387];
static EvtWord wait_event[32];

/**
 * Unit kind of the Smorg Miasma (HP 4, level 0):
 * its size, offsets, damage sounds, status vulnerability, parts and init event.
 */
BattleUnitKind unit_boss_moamoa_tentacle_b = {
    UNIT_SMORG_MIASMA_TENTACLE_B, // unitType
    "btl_un_moamoa_tentacl_b", // name
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
    10, // pbSoftCap
    90, // width
    100, // height
#if VERSION <= VERSION_DEMO_JP
    0,
    85,
#else
    {0, 85}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {280.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {45.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
#endif
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
#if VERSION <= VERSION_DEMO_JP
    0.0f,
    0,
    0,
    280.0f,
    40.0f,
    0.0f,
    45.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    250.0f,
    0.0f,
    45.0f,
    0.0f,
    0.0f,
    35.0f,
    75.0f,
    20.0f,
    -30.0f,
    130.0f,
    0.0f,
#else
    {45.0f, 0.0f, 0.0f}, // bintaHitOffset
    {35.0f, 75.0f, 20.0f}, // kissHitOffset
    {-30.0f, 130.0f, 0.0f}, // cutBaseOffset
#endif
    90.0f, // cutWidth
    100.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    0, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_MOA_G_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Smorg Miasma: 1 damage points off every hit. */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Element reactions of the Smorg Miasma: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the Smorg Miasma (percent multiplier on each status's chance; all 0):
 * immune to sleep, stop, dizzy, poison, confusion, electric shock, burn, freeze, growing huge, shrinking,
 * attack up, attack down, defence up, defence down, allergy, fright, gale force, speed up, slow, dodgy,
 * invisibility and instant kill.
 */
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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "S_1B_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1B_1"},
    {POSE_TYPE_DAMAGE, "D_1B_1"},
    {POSE_TYPE_ACTION, "S_1B_1"},
};

/** Data table of the Smorg Miasma: event handlers for being defeated. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/** Body part of the Smorg Miasma (model c_g_moamoa_b): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {
        1, "btl_un_moamoa_tentacle_b", "c_g_moamoa_b",
        0.0f, 0.0f, 0.0f,
        0.0f, 80.0f, 20.0f,
        0.0f, 100.0f, 20.0f,
        20, 30, 255,
        &defence, &defence_attr,
        kMainPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table,
    },
};

/** Weapon of the Smorg Miasma's attack: 5 damage. */
static BattleWeapon weapon_moamoa_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
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
 * Init event of Smorg's tentacle B: installs its wait, attack,
 * damage and default confusion events and moves its HP gauge to offset (0, 80).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, 0, 80)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of Smorg's tentacle B: runs the default damage handling with LW(10) = -2 and LW(11) = 1. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Death of Smorg's tentacle B: it plays the arm-down sound and the R_1B pose,
 * clears the main body's slot for it (unit work 4 = -1), then, if all three tentacle slots are empty,
 * gives the main body the no-parts defence table and weakness pose table,
 * plays the BIG sounds and V_1 pose on it, and kills the tentacle unit.
 */
// Clear this tentacle's body slot; expose the body only after all three tentacle slots are empty.
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1B"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_LIMB_WORK_BODY, LW(1))
    CALL(btlevtcmd_SetUnitWork, LW(1), MOAMOA_UNIT_WORK_TENTACLE_B, -1)
    CALL(btlevtcmd_GetUnitWork, LW(1), MOAMOA_UNIT_WORK_TENTACLE_A, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, LW(1), MOAMOA_UNIT_WORK_TENTACLE_B, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(btlevtcmd_GetUnitWork, LW(1), MOAMOA_UNIT_WORK_TENTACLE_C, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_SetPartsDefenceTable, LW(1), 1, PTR(&moamoa_defence_no_parts))
                CALL(btlevtcmd_OffPartsAttribute, LW(1), 1, kImmuneToDamageAndStatus)
                CALL(btlevtcmd_AnimeSetPoseTable, LW(1), 1, PTR(moamoa1_weakness_pose_table))
                CALL(btlevtcmd_SetHitOffset, LW(1), 1, 0, 32, 0)
                CALL(btlevtcmd_SetHitCursorOffset, LW(1), 1, 0, 41, 0)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_BIG1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_BIG2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("V_1"))
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/** Phase event of Smorg's tentacle B: does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Attack event of Smorg's tentacle B: runs a selected enemy item-use script when one is available, otherwise runs evt_attack. */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL_CHILD(evt_attack)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Moves another tentacle along with this one's attack: plays the first section of the pose list at LW(1)
 * (sections end at a NULL entry) on part LW(5) of unit LW(0) with the arm-move sound running,
 * waits until the main
 * body (LW(6)) has unit work 0 set back to non-zero (done by evt_attack after its damage check), then plays the
 * next section. Spawned by evt_attack for each of the other two tentacles that is still alive.
 */
// Consume one animation segment, wait for the body's hit signal, then consume the next segment.
static EVT_BEGIN(evt_attack_other_arm)
    SET_INT_READ_ADDR(LW(1))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(15))
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, LW(0), LW(5), LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(0), LW(5))
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, LW(6), MOAMOA_UNIT_WORK_LIMB_HIT, LW(10))
        IF_INT_NOT_EQUAL(LW(10), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(15))
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, LW(0), LW(5), LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(0), LW(5))
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    RETURN
EVT_END()

/** Animation names tentacle A plays, in order, while tentacle B attacks (A holds its pose). */
const char* A_of_B_Attack[] = {
    "S_1A_1",
    NULL,
    NULL,
};

/** Animation names tentacle B plays, in order, for its swing when the target stands in front. */
const char* B_of_B_Attack_front[] = {
    "M_1B_3",
    "A_1B_1",
    "A_1B_2",
    NULL,
    "A_1B_3",
    NULL,
    "A_1B_4",
    "M_1B_4",
    "S_1B_1",
    NULL,
};

/** Animation names tentacle B plays, in order, for its swing when the target stands toward the back. */
const char* B_of_B_Attack_back[] = {
    "M_1B_3",
    "A_1B_5",
    "A_1B_6",
    NULL,
    "A_1B_7",
    NULL,
    "A_1B_8",
    "M_1B_4",
    "S_1B_1",
    NULL,
};

/** Animation names tentacle C plays, in order, while tentacle B attacks (C follows along). */
const char* C_of_B_Attack[] = {
    "M_1C_1",
    "S_1C_2",
    NULL,
    "M_1C_2",
    "S_1C_1",
    NULL,
};

static EVT_BEGIN(evt_attack)
    // Choose the target for the tentacle's swat; a confused tentacle is flustered when there is none.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_moamoa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_moamoa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        RETURN
    END_IF

    // The camera closes in on the tentacle and its target, and the attack is announced.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_moamoa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_moamoa_attack))

    // The swing is chosen by where the target stands: in front (the near side) or toward the back.
    CALL(boss_moamoa_check_zenei_kouei, LW(3), LW(1))
    IF_INT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 0)
    ELSE
        SET_VALUE(LF(0), 1)
    END_IF

    // The main body's limb-hit signal (unit work 0) is cleared and the body is set to its S_1_1 pose; the other
    // two tentacles that are still alive move along.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MOAMOA_LIMB_WORK_BODY, LW(6))
    CALL(btlevtcmd_SetUnitWork, LW(6), MOAMOA_UNIT_WORK_LIMB_HIT, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(6), 1, STRING("S_1_1"))
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_GetUnitWork, LW(6), MOAMOA_UNIT_WORK_TENTACLE_A, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        SET_VALUE(LW(1), PTR(&A_of_B_Attack))
        SET_VALUE(LW(5), 1)
        SPAWN_EVENT(evt_attack_other_arm)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(6), MOAMOA_UNIT_WORK_TENTACLE_C, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        SET_VALUE(LW(1), PTR(&C_of_B_Attack))
        SET_VALUE(LW(5), 1)
        SPAWN_EVENT(evt_attack_other_arm)
    END_IF

    // The tentacle's arm sound starts, and it sweeps back and forth through its swing animation.
    IF_INT_EQUAL(LF(0), 0)
        SET_INT_READ_ADDR(PTR(&B_of_B_Attack_front))
        SET_VALUE(LW(15), 1)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(14))
            WAIT_MS(2000)
            CALL(evt_snd_sfxoff, LW(14))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
    ELSE
        SET_INT_READ_ADDR(PTR(&B_of_B_Attack_back))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(14))
            WAIT_MS(2300)
            CALL(evt_snd_sfxoff, LW(14))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
    END_IF
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    END_LOOP

    // A pause, a heavy arm-slam sound, then the swat comes down.
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(0), 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE3_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        LOOP(0)
            READ_INT(LW(2))
            IF_INT_EQUAL(LW(2), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        END_LOOP
    ELSE
        BEGIN_SIBLING_EVENT
            LOOP(0)
                READ_INT(LW(2))
                IF_INT_EQUAL(LW(2), 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            END_LOOP
            LOOP(0)
                READ_INT(LW(2))
                IF_INT_EQUAL(LW(2), 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            END_LOOP
            SET_VALUE(GF(9), 0)
        END_SIBLING_EVENT
        WAIT_MS(70)
    END_IF

    // The swat's hit is checked: a target that dodges is missed, otherwise it takes the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_moamoa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_moamoa_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_moamoa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(4)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_SWITCH

    // The main body's limb-hit signal is set again, the camera returns and the tentacle finishes its animation.
    CALL(btlevtcmd_SetUnitWork, LW(6), MOAMOA_UNIT_WORK_LIMB_HIT, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    IF_INT_EQUAL(LW(15), 0)
        LOOP(0)
            READ_INT(LW(2))
            IF_INT_EQUAL(LW(2), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        END_LOOP
    ELSE
        SET_VALUE(GF(9), 1)
        WAIT_WHILE(GF(9))
    END_IF
    RETURN
EVT_END()

/**
 * Idle event of Smorg's tentacle B: shows its table pose on part 1,
 * and does the same for the other two tentacles while they still exist.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)

    // BUG: nothing sets LW(6) (the body id) in a wait event, so it is 0 and the other tentacles' ids are
    // read from unit 0's work instead of the body's.
    CALL(btlevtcmd_GetUnitWork, LW(6), MOAMOA_UNIT_WORK_TENTACLE_A, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(10), 1)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(6), MOAMOA_UNIT_WORK_TENTACLE_C, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(10), 1)
    END_IF
    RETURN
EVT_END()
