// Keelhaul Key (muj) battle unit: Green Fuzzy (green_chorobon): its parts, defence/status tables, and attack events.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

static USER_FUNC(chorobon_copy_status);

BattleUnitKind unit_green_chorobon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_chorobon_drain_attack;
static EvtWord attack_event[139];
static BattleUnitSetup entry_chorobon;
static EvtWord friend_event[223];
static BattleWeapon weapon_chorobon_drain_attack_biribiri_counterd;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[14];
static EvtWord rabbit_move[123];
static PoseSoundTiming pose_sound_timing_table[4];
static EvtWord init_event[31];
static EvtWord damage_event[10];
static EvtWord counter_damage_event[41];
static EvtWord return_event[24];
static EvtWord drain_attack_event[887];
static EvtWord wait_event[6];

/**
 * Unit data of the Green Fuzzy: 5 HP, 28x32 units in size; ties together its sounds, parts,
 * status vulnerability, init event and event table.
 */
BattleUnitKind unit_green_chorobon = {
    UNIT_GREEN_FUZZY, // unitType
    "btl_un_green_chorobon", // name
    5, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    19, // level
    0, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    60, // runRate
    9999, // pbSoftCap
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
    {9.0f, 20.0f, 0.0f}, // kissHitOffset
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
    "SFX_ENM_CHORO2_DAMAGED1", // damageSfxName
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

/** Defence of the Green Fuzzy: 0 against every kind of hit. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of the Green Fuzzy: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {90, 90, 80, 100, 80, 100, 100, 65, 100, 70, 100, 70, 100, 65, 65, 85, 85, 100, 65, 100, 100, 95};
#else
static StatusVulnerability regist = {
    85, // sleep
    85, // stop
    80, // dizzy
    100, // poison
    85, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    60, // allergic
    85, // fright
    80, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Parts of the Green Fuzzy: 1 part (model c_chorobon_g). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_green_chorobon", "c_chorobon_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/**
 * Weapon of the Green Fuzzy (drain attack): targets one unit for 3 damage, ignores Defense;
 * used by drain_attack_event.
 */
static BattleWeapon weapon_chorobon_drain_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    0, // counterResistFlags
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
 * Attack event (also its confusion event) of the Green Fuzzy: may use an item;
 * otherwise half the time it drains a target (drain_attack_event).
 * The other half it looks for a free spot behind it (out to x = 170) and then in front of it and,
 * when it finds one, splits into a new Green Fuzzy there (friend_event); with no room it rolls again.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(0)
    SET_VALUE(LW(0), 50)
    ADD(LW(0), 50)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 50)
        CALL_CHILD(PTR(drain_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        IF_INT_LESS_OR_EQUAL(LW(0), 170)
            ADD(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(PTR(friend_event))
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
        SET_VALUE(LW(4), LW(0))
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(4), LW(5))
        IF_INT_LESS(LW(4), 0)
            SUBTRACT(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(PTR(friend_event))
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Spawn setup for the Green Fuzzy (unit_green_chorobon): enemy side, placed off-screen at first;
 * used by friend_event.
 */
static BattleUnitSetup entry_chorobon = {
    &unit_green_chorobon, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * The Green Fuzzy multiplies (its turn's action, from attack_event):
 * it spins faster and faster with the split sounds, hops,
 * and spawns a new Green Fuzzy that takes its current HP and size status (chorobon_copy_status) and jumps,
 * spinning, to the free spot in LW(0) to LW(2), which becomes its home.
 */
static EVT_BEGIN(friend_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_K_1"))
    SET_VALUE(LW(12), 120)
    SET_VALUE(LW(13), 45)
    SET_VALUE(LW(14), 10)
    SET_VALUE(LW(15), 10)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(0)
        WAIT_FRAMES(1)
        SUBTRACT(LW(12), 1)
        IF_INT_LESS_OR_EQUAL(LW(12), 1)
            BREAK_LOOP
        END_IF
        SUBTRACT(LW(14), 1)
        IF_INT_LESS_OR_EQUAL(LW(14), 0)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(13))
            SUBTRACT(LW(15), 1)
            SET_VALUE(LW(14), LW(15))
            IF_INT_EQUAL(LW(15), 7)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_SPLIT1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            END_IF
        END_IF
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 10, FLOAT(0.5f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(7), LW(8), LW(9), FLOAT(20.0f), MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_SPLIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_chorobon), 0)
    CALL(chorobon_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetPos, LW(3), LW(7), LW(8), LW(9))
    CALL(btlevtcmd_JumpSetting, LW(3), 0, 10, FLOAT(0.5f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 10, 0)
        SET_VALUE(LW(12), 0)
        LOOP(0)
            WAIT_FRAMES(1)
            ADD(LW(12), 1)
            IF_INT_GREATER_OR_EQUAL(LW(12), 30)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddRotate, LW(3), 0, 0, 20)
        END_LOOP
        CALL(btlevtcmd_SetRotate, LW(3), 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), LW(1), LW(2), FLOAT(30.0f), MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/**
 * Weapon of the Green Fuzzy (drain attack biribiri counterd): targets one unit; used by drain_attack_event.
 */
static BattleWeapon weapon_chorobon_drain_attack_biribiri_counterd = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, 0, // acHelpMsg..counterResistFlags
    TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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
 * Event table of the Green Fuzzy: counter_damage_event (tags 26, 25, 27, 28, 29, 30, 31, 32, 33, 34, 35,
 * 36).
 */
static BattleDataEntry data_table[] = {
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

/**
 * Hop movement of the Green Fuzzy: hops toward LW(5) to LW(7) in hops of at most LW(8) units,
 * squashing before and stretching after each hop, with the landing sound on the last one.
 */
static EVT_BEGIN(rabbit_move)
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
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(5), LW(6), LW(7), 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
 * Sound cues of the Green Fuzzy: SFX_ENM_CHORO2_MOVE1, SFX_ENM_CHORO2_WAIT1,
 * SFX_ENM_CHORO2_WAIT2 plays at fixed points of the animations CBN_S_1.
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"CBN_S_1", 0.5833333f, NULL, "SFX_ENM_CHORO2_MOVE1", 0, 1},
    {"CBN_S_1", 0.8333333f, NULL, "SFX_ENM_CHORO2_WAIT2", 0, 1},
    {"CBN_S_1", 1.5833334f, NULL, "SFX_ENM_CHORO2_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Init event of the Green Fuzzy: installs its wait, attack, damage and confusion (the attack event) events,
 * its pose sound table and jump sound.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_MOVE1"), 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of the Green Fuzzy: runs the default damage reaction. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Data event (tags 25 to 36) for when the Green Fuzzy is hurt by a counter while attacking:
 * resets its scale, plays the counter damage reaction and, unless that knocked it out for good,
 * hops home (return_event).
 */
static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(PTR(subsetevt_counter_damage))
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL_CHILD(PTR(return_event))
    RETURN
EVT_END()

/** The Green Fuzzy hops back to its home position in hops of 40 (rabbit_move). */
static EVT_BEGIN(return_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 40)
    CALL_CHILD(PTR(rabbit_move))
    RETURN
EVT_END()

/**
 * Drain attack of the Green Fuzzy: it hops up to one target,
 * jumps onto it and drains it (weapon_chorobon_drain_attack), healing itself by the amount drained,
 * then hops home.
 */
static EVT_BEGIN(drain_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_chorobon_drain_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 50)
    // It hops over toward its target in bounds.
    CALL_CHILD(rabbit_move)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    // The camera pulls back and the Green Fuzzy crouches, then leaps high up onto its target.
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 60, 0, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
    SUBTRACT(LW(1), 10)
    SUBTRACT(LW(2), 2)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_drain_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // The leap misses or is blocked: it bounces off in two hops, lands in front of the target and waits a
    // moment.
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_N_1"))
    WAIT_MS(1000)
    GOTO(98)
    // The leap lands. First it checks whether the target is protected by a shock counter.
    LABEL(91)
    SET_VALUE(LF(0), 0)
    CALL(btlevtcmd_PreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_drain_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 17)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(5), 14)
        SET_VALUE(LF(0), 1)
    END_IF
    // If the target shocks it, the Green Fuzzy takes the counter damage instead and the attack ends there.
    IF_INT_EQUAL(LF(0), 1)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_chorobon_drain_attack))
        SET_VALUE(LW(9), PTR(&weapon_chorobon_drain_attack_biribiri_counterd))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    // It clings to the target, which shakes while the Green Fuzzy latches on.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetBodyId, LW(3), LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(0), POSE_TYPE_DAMAGE_DRAINED)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 89)
        SET_VALUE(LW(1), 1)
        SET_VALUE(LW(2), 2)
        SET_VALUE(LW(5), LW(2))
        LABEL(20)
        SUBTRACT(LW(5), 1)
        IF_INT_LESS_OR_EQUAL(LW(5), 0)
            SET_VALUE(LW(5), LW(2))
            IF_INT_EQUAL(LW(1), 0)
                SET_VALUE(LW(1), 1)
            ELSE
                SET_VALUE(LW(1), 0)
            END_IF
        END_IF
        CALL(btlevtcmd_SetDispOffset, LW(3), LW(1), 0, 0)
        WAIT_FRAMES(1)
        SUBTRACT(LW(0), 1)
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
    END_INLINE_EVENT
    // It drains the target for about 90 frames (CBN_A_2), then squashes down.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(14), 20)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), -20)
    END_IF
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(14))
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.5f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_A_2"))
    WAIT_FRAMES(90)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_R_2"))
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    LOOP(4)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, 0, FLOAT(-0.19921875f), 0)
    END_LOOP
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(0.19921875f), FLOAT(1.0f))
    LOOP(20)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.009765625f), FLOAT(0.049804688f), 0)
    END_LOOP
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    // The Action Command defence result is applied and it drains HP from the target.
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_chorobon_drain_attack))
    SET_VALUE(LW(9), PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetHpDamage, LW(3), LW(10))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    // If it drained anything it swells up, and then it heals itself by that amount with a recovery effect.
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.5f), FLOAT(0.5f), 0)
    END_IF
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.07910156f), FLOAT(0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.31933594f), FLOAT(-0.4794922f), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.31933594f), FLOAT(0.4794922f), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_W_1"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        LOOP(20)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.024414062f), FLOAT(-0.024414062f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(1))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        ADD(LW(6), 45)
        ADD(LW(7), 10)
        IF_INT_EQUAL(LW(9), PTR(&weapon_chorobon_drain_attack))
            CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, LW(1), LW(10))
            CALL(evt_eff, STRING(""), STRING("recovery"), 7, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        ELSE
            CALL(btlevtcmd_RecoverFp, UNIT_ID_SELF, LW(1), LW(10))
            CALL(evt_eff, STRING(""), STRING("recovery"), 8, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        END_IF
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_IF
    LOOP(2)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        // It hops home in two bounces.
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK3"), 0, 0, -1, -1, 0, 0)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_KEEP)
        WAIT_FRAMES(16)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, 0, 0, 0, -1, -1, 0, 0)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 6, MOVE_SOUND_KEEP)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    GOTO(98)
    // The camera resets and the Green Fuzzy gets back to its starting spot.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL_CHILD(return_event)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Script helper: copies the current HP (capped to the max HP),
 * size change turns and strength of unit args[0] to unit args[1] and resets the second unit's HP gauge;
 * friend_event uses it so a newly split Green Fuzzy starts with its parent's HP and size.
 */
static USER_FUNC(chorobon_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    // Copy HP and size-change state only; the other status fields are left unchanged.
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

/** Wait (idle) event of the Green Fuzzy, installed by its init (btlevtcmd_SetEventWait): sets the idle pose from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

