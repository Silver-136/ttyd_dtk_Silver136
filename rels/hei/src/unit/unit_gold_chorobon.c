// Gold Fuzzy (gold_chorobon): battle unit used in Petal Meadows.

#include "version.h"
#include "battle/battle_unit.h"
#include "unit/unit_chorobon_gundan.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_msg.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_CHOROGUN_CALLED 0 // 1 once damage_event has called the chorogun unit (HP 1-7: entry_chorogun, call_chorogun_event)
#define UNIT_WORK_CHOROGUN_GONE 1   // 1 once the chorogun has been sent off (escape_chorogun_event, or damage_event when this unit drops to 0 HP while the chorogun is out); stops it running twice
#define UNIT_WORK_CHOROGUN 2        // unit id of the chorogun unit it called

USER_FUNC(get_gold_chorobon_id);
extern EvtWord chorogun_entry[];
extern BattleUnitKind unit_chorobon_gundan;
extern EvtWord chorogun_dead_reward[];

BattleUnitKind unit_gold_chorobon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[15];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_gold_chorobon_attack;
static PoseSoundTiming pose_sound_timing_table[4];
EvtWord gold_chorobon_rabbit_move[123];
BattleUnitSetup entry_chorogun;
EvtWord call_chorogun_event[67];
#if VERSION <= VERSION_DEMO_US
extern EvtWord escape_chorogun_event[];
#else
EvtWord escape_chorogun_event[50];
#endif
static EvtWord init_event[43];
#if VERSION <= VERSION_DEMO_US
static EvtWord damage_event[108];
#else
static EvtWord damage_event[131];
#endif
static EvtWord counter_damage_event[30];
static EvtWord return_event[24];
static EvtWord attack_event[500];
static EvtWord wait_event[6];

/**
 * Unit kind of the Gold Fuzzy:
 * 10 HP, level 27, with its size, sounds and the links to its parts, status vulnerability and scripts.
 */
BattleUnitKind unit_gold_chorobon = {
    UNIT_GOLD_FUZZY, // unitType
    "btl_un_gold_chorobon", // name
    10, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    27, // level
    0, // bonusExp
    10, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    5, // pbSoftCap
    28, // width
    32, // height
    {5, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 20.8f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    28.0f, // cutWidth
    32.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_G_CHORO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitLeader, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Gold Fuzzy's body per damage type: 0 against every damage type. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of the Gold Fuzzy's body: none, so every damage type is dealt normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the Gold Fuzzy, as a percent of the normal chance:
 * immune to sleep, stop, dizzy, confuse, freeze, tiny, attack down, defense down, allergic, fright,
 * gale force, slow, instant kill.
 */
static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    100, // poison
    0, // confuse
    100, // electric
    100, // burn
    0, // freeze
    100, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    0, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};

/**
 * Pose table of the Gold Fuzzy:
 * the animation played for each pose type (idle, hurt, walk, run, attack and so on).
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "CBN_N_1"},
    {POSE_TYPE_STOP, "CBN_Y_1"},
    {POSE_TYPE_FREEZE, "CBN_Y_1"},
    {POSE_TYPE_CONFUSE, "CBN_K_1"},
    {POSE_TYPE_POISON, "CBN_X_1"},
    {POSE_TYPE_DIZZY, "CBN_X_1"},
    {POSE_TYPE_NORMAL, "CBN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "CBN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "CBN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "CBN_S_1"},
    {POSE_TYPE_DAMAGE, "CBN_D_1"},
    {POSE_TYPE_ATTACK, "CBN_A_1"},
    {POSE_TYPE_RUN, "CBN_R_1"},
    {POSE_TYPE_WALK, "CBN_W_1"},
    {POSE_TYPE_DANGER, "CBN_X_1"},
    {POSE_TYPE_PERIL, "CBN_X_1"},
    {POSE_TYPE_TALK, "CBN_T_1"},
    {POSE_TYPE_ACTION, "CBN_S_1"},
};

/**
 * Event table of the Gold Fuzzy:
 * the reaction when it is hurt while attacking, nothing special on a First Strike.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, counter_damage_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Body part of the Gold Fuzzy: a single part using the model "c_chorobon_k". */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gold_chorobon", "c_chorobon_k", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Gold Fuzzy's attack: 1 damage to a single target, a jump-style hit. */
BattleWeapon weapon_gold_chorobon_attack = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Sound timing of the Gold Fuzzy:
 * the sound effect played at set points of its idle, walk and run animations.
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"CBN_S_1", 0.5833333f, NULL, "SFX_BOSS_G_CHORO_MOVE1", 0, 1},
    {"CBN_S_1", 0.8333333f, NULL, "SFX_BOSS_G_CHORO_WAIT2", 0, 1},
    {"CBN_S_1", 1.5833334f, NULL, "SFX_BOSS_G_CHORO_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Hop-walk used by the Gold Fuzzy's attack: squashes,
 * then jumps in steps of LW(8) units toward x=LW(5) (height LW(6), depth LW(7)), squashing between hops,
 * and finishes with the attack jump onto the target with its attack sound.
 */
EVT_BEGIN(gold_chorobon_rabbit_move)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LABEL(0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(11), LW(5))
    SUBTRACT(LW(11), LW(0))
    SET_VALUE(LW(12), LW(11))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    IF_INT_GREATER(LW(12), LW(8))
        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
            ADD(LW(0), LW(8))
        ELSE
            SUBTRACT(LW(0), LW(8))
        END_IF
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(6), LW(2), 0, MOVE_SOUND_FROM_TABLE)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        GOTO(0)
    ELSE
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(5), LW(6), LW(7), 0, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_G_CHORO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_IF
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Setup entry that adds the Fuzzy horde unit on the enemy side at (75, 0, 0)
 * when the Gold Fuzzy calls for backup.
 */
BattleUnitSetup entry_chorogun = {
    &unit_chorobon_gundan, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {75.0f, 0.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Battle script run when the Gold Fuzzy calls its friends:
 * it shows the wave animation, the camera pans in on it and it says stg1_hei_43, Mario is startled,
 * and chorogun_appear drops the 1000-Fuzzy horde into place.
 */
EVT_BEGIN(call_chorogun_event)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_S_1"))
    WAIT_MS(1000)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 120, 40, 200, 120, -10, -500, 30, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg1_hei_43"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
    END_SIBLING_EVENT
    CALL(chorogun_appear)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_MARIO, 1)
    RETURN
EVT_END()

/**
 * Script for the Gold Fuzzy's end: after two seconds it releases the Fuzzy Horde (paying its reward),
 * sends all the Fuzzies running off the stage and removes the horde unit;
 * marks the unit work so it only happens once (JP and later).
 */
EVT_BEGIN(escape_chorogun_event)
    WAIT_MS(2000)
    CALL(get_gold_chorobon_id, LW(4))
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(4), UNIT_WORK_CHOROGUN_GONE, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, LW(4), UNIT_WORK_CHOROGUN_GONE, 1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(10))
    CALL_CHILD(PTR(chorogun_dead_reward))
#endif
    CALL(chorogun_escape_all)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetUnitWork, LW(4), UNIT_WORK_CHOROGUN, LW(3))
    CALL(btlevtcmd_KillUnit, LW(3), 0)
#if VERSION <= VERSION_DEMO_US
    CALL(btlevtcmd_SetUnitWork, LW(4), UNIT_WORK_CHOROGUN_GONE, 1)
#endif
    RETURN
EVT_END()

/**
 * Init event of the Gold Fuzzy: installs wait, attack, damage and default confusion events,
 * starts the Fuzzy horde setup (chorogun_entry),
 * clears unit work 0 and 1 and sets the boss hop and landing sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL_CHILD(PTR(chorogun_entry))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, 0)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_BOSS_G_CHORO_MOVE1"), STRING("SFX_BOSS_G_CHORO_LANDING1"))
    RETURN
EVT_END()

/**
 * Damage event of the Gold Fuzzy: while the swarm is out (work 0)
 * and he is hit by a counter-type attack (512 without 2048),
 * the swarm unit is removed with its reward (chorogun_dead_reward in JP and later) and the horde flees.
 * After the default damage handling, the first time his HP drops to 7
 * or less (but not 0) he spawns the Fuzzy swarm (entry_chorogun) and calls it in (call_chorogun_event);
 * unit work 0 marks that it is out.
 */
// Summon the horde once at 1-7 HP; a non-reviving lethal hit sends an already-called horde away first.
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(0))
    CALL(btlevtcmd_GetUnitId, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), -1)
#endif
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
#if VERSION > VERSION_DEMO_US
                        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, 1)
#endif
                        CALL(btlevtcmd_WaitAttackEnd)
#if VERSION > VERSION_DEMO_US
                        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(10))
                        CALL_CHILD(PTR(chorogun_dead_reward))
                        SET_VALUE(LW(10), -2)
#endif
                        CALL(chorogun_escape_all)
                        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(3))
                        CALL(btlevtcmd_KillUnit, LW(3), 0)
#if VERSION <= VERSION_DEMO_US
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, 1)
#endif
                    END_IF
                END_IF
            END_IF
        END_IF
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(btldefaultevt_Damage)
#else
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
#endif
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 7)
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                CALL(btlevtcmd_WaitAttackEnd)
                CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_chorogun), 0)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(3))
                CALL_CHILD(PTR(call_chorogun_event))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, 1)
                GOTO(99)
            END_IF
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Damage event used for the Gold Fuzzy's counter-damage case:
 * restores its scale, runs the shared counter damage handling (subsetevt_counter_damage) and,
 * if it is still alive, hops back home with return_event.
 */
static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(PTR(subsetevt_counter_damage))
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(return_event))
    END_IF
    RETURN
EVT_END()

/** The Gold Fuzzy hops back to its home position (in hops of at most 40 units) with gold_chorobon_rabbit_move. */
static EVT_BEGIN(return_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 40)
    CALL_CHILD(PTR(gold_chorobon_rabbit_move))
    RETURN
EVT_END()

/**
 * Attack event of the Gold Fuzzy: hops over to a target and jumps onto it (weapon_gold_chorobon_attack);
 * the pre-damage check decides between a dodge and a hit with the action-command result, and it hops back home.
 */
static EVT_BEGIN(attack_event)
    // It may use an enemy item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: it picks one for its attack; with none, a confused one flusters, otherwise the attack ends.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_gold_chorobon_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gold_chorobon_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gold_chorobon_attack))

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gold_chorobon_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)

    // It hops over to the target in two hops.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 50)
    CALL_CHILD(PTR(gold_chorobon_rabbit_move))
    WAIT_FRAMES(20)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 60, 0, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 2)

    // It jumps onto the target; the pre-damage check decides between a dodge and a hit.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gold_chorobon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: it lands beside the target (which dodges for some kinds of miss) and waits.
    LABEL(90)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(3.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 5)
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(1.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_N_1"))
    WAIT_MS(1000)
    GOTO(98)

    // Hit: it squashes against the target, the attack lands for its damage
    // and it bounces back in smaller hops.
    LABEL(91)
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.07910156f), FLOAT(-0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gold_chorobon_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gold_chorobon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.07910156f), FLOAT(0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 10, MOVE_SOUND_FROM_TABLE)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_A_1"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(16)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 6, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_S_1"))
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(10)
    GOTO(98)

    // It hops back to its home position.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL_CHILD(PTR(return_event))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait (idle) event of the Gold Fuzzy: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

// Return the first occupied slot whose initial kind is Gold Fuzzy, or -1 if none exists.
USER_FUNC(get_gold_chorobon_id) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_GOLD_FUZZY) {
            break;
        }
    }
    if (i != 64) {
        evtSetValue(event, args[0], i);
    } else {
        evtSetValue(event, args[0], -1);
    }
    return EVT_RETURN_DONE;
}
