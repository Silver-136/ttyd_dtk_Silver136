// Pit of 100 Trials battle unit: the Bob-omb (`bomhei`): parts, stats, poses and AI scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_bomhei.h"

BattleUnitKind unit_bomhei;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_spark[18];
static BattleDataEntry data_table[2];
static BattleDataEntry data_table_spark[2];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
static BattleWeapon weapon_bomhei_attack;
static BattleWeapon weapon_bomhei_bomb;
static EvtWord bomb_event[106];
static EvtWord init_event[42];
static EvtWord damage_event[58];
static EvtWord change_spark_event[70];
static EvtWord attack_event[440];
static EvtWord wait_event[6];
static EvtWord damage_event_spark[25];

#if VERSION <= VERSION_DEMO_JP
static EvtWord attack_event_spark[292];
#else
static EvtWord attack_event_spark[288];
#endif

static EvtWord wait_event_spark[27];

/** Unit data of the Bob-omb: stats, sounds, attributes, parts, init script and data table. */
BattleUnitKind unit_bomhei = {
    UNIT_BOB_OMB, // unitType
    "btl_un_bomhei", // name
    4, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    12, // level
    0, // bonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    30, // height
    {4, 30}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {11.0f, 16.0f, 0.0f}, // kissHitOffset
    {2.0f, 15.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_BOMB_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Damage reduction of the Bob-omb: defence 1 against every attack type. */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Element response of the Bob-omb: none, every attack type is treated normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Pose table of the Bob-omb: battle pose types mapped to animation names. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "BOM_N_1"},
    {POSE_TYPE_STOP, "BOM_Y_1"},
    {POSE_TYPE_FREEZE, "BOM_Y_1"},
    {POSE_TYPE_CONFUSE, "BOM_K_1"},
    {POSE_TYPE_POISON, "BOM_X_1"},
    {POSE_TYPE_DIZZY, "BOM_X_1"},
    {POSE_TYPE_NORMAL, "BOM_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "BOM_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BOM_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "BOM_S_1"},
    {POSE_TYPE_DAMAGE, "BOM_D_1"},
    {POSE_TYPE_ATTACK, "BOM_A_1"},
    {POSE_TYPE_RUN, "BOM_R_1"},
    {POSE_TYPE_WALK, "BOM_W_1"},
    {POSE_TYPE_DANGER, "BOM_X_1"},
    {POSE_TYPE_PERIL, "BOM_X_1"},
    {POSE_TYPE_TALK, "BOM_T_1"},
    {POSE_TYPE_ACTION, "BOM_S_1"},
};

/** Pose table of the Bob-omb, fuse lit: battle pose types mapped to animation names. */
static PoseTableEntry pose_table_spark[] = {
    {POSE_TYPE_SLEEP, "BOM_N_2"},
    {POSE_TYPE_STOP, "BOM_Y_2"},
    {POSE_TYPE_FREEZE, "BOM_Y_2"},
    {POSE_TYPE_CONFUSE, "BOM_K_2"},
    {POSE_TYPE_POISON, "BOM_X_2"},
#if VERSION <= VERSION_DEMO_JP
    {POSE_TYPE_DIZZY, "BSA_X_2"},
#else
    {POSE_TYPE_DIZZY, "BOM_X_2"},
#endif
    {POSE_TYPE_NORMAL, "BOM_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "BOM_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BOM_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "BOM_S_2"},
    {POSE_TYPE_DAMAGE, "BOM_D_2"},
    {POSE_TYPE_ATTACK, "BOM_A_2"},
    {POSE_TYPE_RUN, "BOM_R_2"},
    {POSE_TYPE_WALK, "BOM_W_2"},
    {POSE_TYPE_DANGER, "BOM_X_2"},
    {POSE_TYPE_PERIL, "BOM_X_2"},
    {POSE_TYPE_TALK, "BOM_T_2"},
    {POSE_TYPE_ACTION, "BOM_S_2"},
};

/** Data table of the Bob-omb: event handlers keyed by tag. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Data table of the Bob-omb, fuse lit: event handlers keyed by tag. */
static BattleDataEntry data_table_spark[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/** Body part of the Bob-omb: hit position, size, defence tables and attributes. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_bomhei", "c_bomhey", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, kCounterExplosive, pose_table},
};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {80, 80, 70, 0, 80, 150, 100, 70, 100, 80, 100, 80, 100, 80, 80, 90, 80, 100, 80, 100, 100, 95};
#else
static StatusVulnerability regist = {
    70, // sleep
    100, // stop
    70, // dizzy
    0, // poison
    70, // confuse
    150, // electric
    100, // burn
    100, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    90, // fright
    80, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Attack data for the Bob-omb's body bump: 2 damage. */
static BattleWeapon weapon_bomhei_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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

/** Attack data for the Bob-omb's blast once its fuse is lit: 5 damage. */
static BattleWeapon weapon_bomhei_bomb = {
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
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_BOMB_FLIPPABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 25, 25, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(bomb_event)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 15, 0)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kNoShadow)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPARK_SFX, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_snd_sfxoff, LW(0))
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPARK_SFX, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENM_BOMB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("sandars"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_StageDispellFog)
    CALL(btlevtcmd_GetCoin, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_MS(1000)
    IF_INT_EQUAL(LW(15), 1)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Init script of the Bob-omb: registers its wait, attack, damage and confusion handlers,
 * sets unit work 0 (the fuse sound id) to -1 and sets its run and walk sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPARK_SFX, -1)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE2"), STRING("SFX_ENM_BOMB_MOVE2"), 0, 4, 4)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE2"), STRING("SFX_ENM_BOMB_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage script of the Bob-omb.
 * A final-hit result (flag 256) of kind 24, 25 or 27 (a fire, explosion or electric hit) sets it off at once
 * (bomb_event).
 * Otherwise it takes the default damage and, if it is still able to act and not in STATUS_INSTANT_KILL (KO),
 * lights its fuse (change_spark_event) only for a final hit that dealt at least 1 HP.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
        SWITCH(LW(0))
            CASE_OR_EQUAL(DAMAGE_CODE_FIRE)
            CASE_OR_EQUAL(DAMAGE_CODE_ELECTRIC)
            CASE_OR_EQUAL(DAMAGE_CODE_EXPLOSION)
            SET_VALUE(LW(15), 1)
            CALL_CHILD(bomb_event)
            RETURN
            CASE_END
        END_SWITCH
    END_IF
    CALL_CHILD(btldefaultevt_Damage)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(change_spark_event)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Lights the fuse after a damaging hit: if the hit took at least 1 HP it plays SFX_ENM_BOMB_SPARK1,
 * switches to the spark wait, attack and damage scripts, pose table and data table,
 * and sets counter attribute 8192 on part 1.
 */
static EVT_BEGIN(change_spark_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetHpDamage, UNIT_ID_SELF, LW(15))
    IF_INT_GREATER_OR_EQUAL(LW(15), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_SPARK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_spark))
        CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_spark))
        CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_spark))
        CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event_spark))
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_spark))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
        CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterVolatileExplosive)
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_spark))
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Attack script of the Bob-omb: a body bump.
 * After an item check it picks a target, hops twice, runs to it and, if the bump connects,
 * deals the damage and gets knocked back in two hops; a miss makes it pass the target and then it walks home.
 * When confused with no target it is flustered. The demo builds use longer waits and a slower walk home.
 */
static EVT_BEGIN(attack_event)

    // Use an item if the item-use check selects one; otherwise pick a target (a confused Bob-omb with nobody to bump gets
    // flustered).
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_bomhei_attack), LW(3), LW(4))
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
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
#else
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
#endif
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_bomhei_attack))

    // Declare the attack.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // It hops twice on the spot, then runs up to the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 10)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // Find out whether the bump connects.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_bomhei_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: it passes the target in three slowing steps and stands there for a moment.
    LABEL(90)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 90)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BOM_N_1"))
#if VERSION <= VERSION_DEMO_JP
    WAIT_MS(1000)
#else
    WAIT_MS(500)
#endif
    GOTO(98)

    // Hit: the damage is dealt and the Bob-omb is knocked back in two hops.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_bomhei_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_bomhei_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(2.0f), FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(1.0f), FLOAT(0.5f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)

    // Walk back home.
    LABEL(98)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE4"), STRING("SFX_ENM_BOMB_MOVE4"), 0, 10, 10, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
#else
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
#endif
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script: plays the Bob-omb's idle pose from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Damage script of the Bob-omb with its fuse lit:
 * a result with the final-hit flag (256) detonates it (bomb_event), otherwise the default damage handling runs.
 */
static EVT_BEGIN(damage_event_spark)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(15), 1)
        CALL_CHILD(bomb_event)
    ELSE
        CALL_CHILD(btldefaultevt_Damage)
    END_IF
    RETURN
EVT_END()

/**
 * Attack script of the lit Bob-omb: it runs at its target like the normal attack,
 * then explodes (bomb_event) whether the blast hits or misses,
 * dealing the bomb weapon's damage to the target when it connects.
 */
static EVT_BEGIN(attack_event_spark)

    // Use an item if the item-use check selects one; otherwise pick a target (a confused Bob-omb with nobody to hit gets
    // flustered).
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_bomhei_bomb), LW(3), LW(4))
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
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
#else
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
#endif
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_bomhei_bomb))

    // Declare the attack; the lit Bob-omb hops twice and runs at the target like the normal bump.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 10)
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
#else
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
#endif
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_bomhei_bomb), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: it runs past the target and blows up anyway.
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 500)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    SET_VALUE(LW(15), 0)
    CALL_CHILD(bomb_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN

    // Hit: the blast damages the target, then the Bob-omb explodes.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_bomhei_bomb))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_bomhei_bomb), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SET_VALUE(LW(15), 0)
    CALL_CHILD(bomb_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN

    // Wait for the blast to settle.
    LABEL(98)
    WAIT_MS(1000)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Wait script of the lit Bob-omb: plays the idle pose and, if no fuse sound is playing yet (unit work 0 is -1),
 * starts the looping SFX_ENM_BOMB_WAIT1 and remembers its handle in unit work 0.
 */
static EVT_BEGIN(wait_event_spark)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPARK_SFX, LW(15))
    IF_INT_EQUAL(LW(15), -1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BOMB_WAIT1"), EVT_NULLPTR, 0, LW(15))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPARK_SFX, LW(15))
    END_IF
    RETURN
EVT_END()

