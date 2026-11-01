// Pit of 100 Trials battle unit: the Moon Cleft (`sinemon`): parts, stats, poses and AI scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_sinemon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_sinemon_attack;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static BattleDataEntry data_table[4];
static EvtWord init_event[42];
static EvtWord dmg_avoid_counter_thorn_head_event[76];
static EvtWord damage_event[58];
static EvtWord dmg_turn_event[96];
static EvtWord wakeup_event[104];
static EvtWord attack_event[454];
static EvtWord wait_event[6];

/**
 * Unit data of the Moon Cleft in the Pit of 100 Trials: 6 HP, level 26;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_sinemon = {
    UNIT_MOON_CLEFT, // unitType
    "btl_un_sinemon", // name
    6, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    26, // level
    0, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    26, // width
    32, // height
    {4, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 24.0f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    32.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_EMON_DAMAGED1", // damageSfxName
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

/**
 * Defence of the Moon Cleft: 5 against normal, ice, explosion and electric hits;
 * immune to fire (defence 99 cancels the hit).
 */
static BattleUnitDefense defence = {{5, 99, 5, 5, 5}};

/**
 * Damage modifiers of the Moon Cleft per element (normal, fire, ice, explosion, electric): fire: immune.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Defence of the Moon Cleft while flipped over against each element (normal, fire, ice, explosion,
 * electric): none, so every hit does its full damage.
 */
static BattleUnitDefense defence_turn = {0};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {30, 70, 30, 100, 30, 100, 0, 50, 100, 50, 100, 50, 100, 60, 60, 30, 10, 100, 60, 100, 100, 95};
#else
static StatusVulnerability regist = {
    30, // sleep
    80, // stop
    30, // dizzy
    100, // poison
    30, // confuse
    100, // electric
    0, // burn
    50, // freeze
    100, // huge
    60, // tiny
    100, // attackUp
    60, // attackDown
    100, // defenseUp
    100, // defenseDown
    50, // allergic
    30, // fright
    20, // galeForce
    100, // fast
    50, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Part of the Moon Cleft: its one body part with hit box, defence tables and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_sinemon", "c_sinemon", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsBombFlippable, kCounterTopSpiky, pose_table},
};

/** Weapon of the Moon Cleft's charge at the target: damage 5. */
static BattleWeapon weapon_sinemon_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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
 * Pose table of the Moon Cleft: animation names for each pose type (idle, walk, hurt, attack and so on).
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "SIN_N_1"},
    {POSE_TYPE_STOP, "SIN_Y_1"},
    {POSE_TYPE_FREEZE, "SIN_Y_1"},
    {POSE_TYPE_CONFUSE, "SIN_K_1"},
    {POSE_TYPE_POISON, "SIN_I_1"},
    {POSE_TYPE_DIZZY, "SIN_I_1"},
    {POSE_TYPE_NORMAL, "SIN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "SIN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "SIN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "SIN_A_1a"},
    {POSE_TYPE_DAMAGE, "SIN_D_1"},
    {POSE_TYPE_ATTACK, "SIN_A_1a"},
    {POSE_TYPE_WALK, "SIN_W_1"},
    {POSE_TYPE_RUN, "SIN_R_1"},
    {POSE_TYPE_DANGER, "SIN_I_1"},
    {POSE_TYPE_PERIL, "SIN_I_1"},
    {POSE_TYPE_TALK, "SIN_T_1"},
    {POSE_TYPE_ACTION, "SIN_S_1"},
};

/**
 * Pose table of the Moon Cleft while flipped over:
 * animation names for each pose type (idle, walk, hurt, attack and so on).
 */
static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "SIN_N_2"},
    {POSE_TYPE_STOP, "SIN_Y_2"},
    {POSE_TYPE_FREEZE, "SIN_Y_2"},
    {POSE_TYPE_CONFUSE, "SIN_K_2"},
    {POSE_TYPE_POISON, "SIN_K_2"},
    {POSE_TYPE_DIZZY, "SIN_K_2"},
    {POSE_TYPE_NORMAL, "SIN_D_2"},
    {POSE_TYPE_DAMAGE_FIRE, "SIN_D_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "SIN_D_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "SIN_D_2"},
    {POSE_TYPE_DAMAGE, "SIN_D_2"},
    {POSE_TYPE_DANGER, "SIN_K_2"},
    {POSE_TYPE_PERIL, "SIN_K_2"},
    {POSE_TYPE_TALK, "SIN_D_2"},
    {POSE_TYPE_ACTION, "SIN_D_2"},
};

/**
 * Data table of the Moon Cleft: event scripts and values the battle looks up by tag (damage reactions,
 * dodge, first strike and so on).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Init event of the Moon Cleft: installs its wait, attack, damage and confusion events and sets its walk,
 * run and jump sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), STRING("SFX_ENM_EMON_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), STRING("SFX_ENM_EMON_MOVE2"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_JUMP1"), STRING("SFX_ENM_EMON_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Waits half a second, then shows the toge_flush spike flash on the Moon Cleft's head;
 * the flash sits higher and farther out for a big (STATUS_HUGE)
 * or small (STATUS_TINY) Moon Cleft than for a normal one.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
        ADD(LW(1), 45)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 7)
        ADD(LW(1), 25)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(1), 35)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Damage event of the Moon Cleft: when it takes damage it plays the SIN_D_1 flinch pose,
 * and if it is currently flipped over (over-turn count of 1 or more) it plays SIN_D_2
 * and bounces twice on the spot, then runs the default damage handling.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(15))
    IF_INT_GREATER_OR_EQUAL(LW(15), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIN_D_1"))
    END_IF
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIN_D_2"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, MOVE_SOUND_FROM_TABLE)
    END_IF
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Flips the Moon Cleft onto its back (a hit that overturns it):
 * switches the part to the turned defence table and pose table,
 * sets the flipped status flag with an over-turn count of 2, plays the SIN_D_2 pose with a hop,
 * lowers it to height 7 and its hit offset by 10,
 * and turns off the part's counter attribute (the spikes no longer counter).
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("SIN_D_2"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 7, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 7, LW(2))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -10, 0)
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    RETURN
EVT_END()

/**
 * Flips the Moon Cleft back onto its feet: counts the over-turn down by one,
 * restores the normal defence and pose tables, clears the flipped flag,
 * returns to height 0 and the SIN_S_1 pose with a hop and the INSIDE2 sound,
 * and turns the counter attribute back on.
 */
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("SIN_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, 0, 0)
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Attack event of the Moon Cleft: if flipped over it only wakes up (wakeup_event).
 * Otherwise it uses an enemy item if it has one, else picks a target with weapon_sinemon_attack,
 * backs away with a short hop, then charges at the target (dash sound, speed 5).
 * A miss runs it past the target; a hit shakes the camera
 * and applies the damage with the action command defence check,
 * after which it jumps back past the target and returns to its place.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        // A flipped-over Moon Cleft only gets back on its feet.
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        // Pick a target for the attack; a confused unit that has none flusters.
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
        WAIT_MS(800)
    ELSE
        // It uses an enemy item instead if it has one.
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sinemon_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sinemon_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sinemon_attack))

    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sinemon_attack))

    // Back away from the target with a short hop.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)

    // Charge at the target at full speed.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(750)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_DASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    DELETE_EVENT(LW(15))

    // Find out whether the attack will connect or miss.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sinemon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // It runs past the target.
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_MS(500)
    GOTO(98)

    // The attack connects.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sinemon_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sinemon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))

    // After the hit it jumps back over the target and returns to its place.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(1.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)

    // Return to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)

    // The attack is over; back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait event of the Moon Cleft: hands part 1 back to its pose table (the idle pose for its current state). */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

