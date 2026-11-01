// unit_hinnya: Spinia, battle unit, one of the three spinning spiked enemies (Spania, Spunia, Spinia).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_hinnya;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_hannya_attack;
static PoseTableEntry pose_table[18];
static EvtWord init_event[25];
static EvtWord dmg_avoid_counter_thorn_head_event[68];
static EvtWord damage_event[10];
static EvtWord attack_event[455];
static EvtWord wait_event[6];

/**
 * Unit data of the Spinia: 3 HP and level 7, with its parts, status vulnerability, init event and event
 * data table.
 */
BattleUnitKind unit_hinnya = {
    UNIT_SPINIA, // unitType
    "btl_un_hinnya", // name
    3, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    7, // level
    0, // bonusExp
    1, // bonusCoin
    60, // bonusCoinRate
    0, // baseCoin
    80, // runRate
    9999, // pbSoftCap
    26, // width
    30, // height
    {3, 25}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {13.0f, 19.5f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_HANNYA_DAMAGED1", // damageSfxName
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

/**
 * Defense of the Spinia against normal, fire, ice, explosion and electric attacks: none, so it takes full
 * damage from every element.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Damage-type reactions of the Spinia: none, so every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {100, 120, 100, 150, 100, 150, 150, 100, 150, 100, 150, 100, 150, 100, 100, 120, 120, 150, 100, 0, 0, 150};
#else
static StatusVulnerability regist = {
    100, // sleep
    120, // stop
    100, // dizzy
    150, // poison
    100, // confuse
    150, // electric
    150, // burn
    100, // freeze
    150, // huge
    100, // tiny
    150, // attackUp
    100, // attackDown
    150, // defenseUp
    100, // defenseDown
    100, // allergic
    110, // fright
    110, // galeForce
    150, // fast
    100, // slow
    0, // dodgy
    0, // invisible
    150, // ohko
};
#endif

/**
 * Event data table of the Spinia: no custom reactions, so the standard battle handlers run for every
 * event.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Parts of the Spinia: a single body part (part 1, model c_hannya_n), each with its defense tables,
 * attributes and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_hinnya", "c_hannya_n", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 37.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Weapon record of the Spinia's charge: 1 damage and counts as a hammer-type hit. */
static BattleWeapon weapon_hannya_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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

/** Pose table of the Spinia: maps each battle pose (idle, hurt, attacking and so on) to its animation. */
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
    {POSE_TYPE_DAMAGE_GUARDED, "A_3"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Spike-counter effect, not used: the Spinia's data table does not list it (the Spania and Spunia register
 * theirs as
 * data event 37, run when an attacker is hurt by the spiked head):
 * plays pose type 31, waits 30 frames, then flashes the "toge_flush" spike effect above the unit for 60 frames.
 * The effect sits 53 units up when the unit is Huge (status 10), 33 when Tiny (status 11) and 43 otherwise.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE_GUARDED)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 53)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 33)
        GOTO(90)
    END_IF
    ADD(LW(1), 43)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
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
 * Attack event of the Spinia, a single spinning charge. It uses the Spania's weapon, weapon_hannya_attack.
 * During a first strike it waits 30 frames and takes the first-strike target;
 * otherwise it uses an enemy item if it holds a usable one, else picks a target.
 * It shakes in place for 20 frames with the charge sound,
 * rushes along the floor to the target (a dodge plays the avoid reaction,
 * a hit takes the guard result and the damage) and carries on 500 units past it.
 * It then reappears at x = 300 and glides back to its home position in four slowing steps.
 * With no target, a confused unit runs subsetevt_confuse_flustered.
 */
static EVT_BEGIN(attack_event)
    // Use the target of the opening first attack if there was one; otherwise use an enemy item, or choose a target.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_hannya_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_hannya_attack), LW(3), LW(4))
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
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_hannya_attack))
    // The attack is announced.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_hannya_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(14))
    // The mask shakes in place, building up for its charge.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HANNYA_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    SET_VALUE(LW(15), 0)
    LOOP(0)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 1, 0, 0)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, -1, 0, 0)
        ADD(LW(15), 1)
        IF_INT_GREATER_OR_EQUAL(LW(15), 10)
            BREAK_LOOP
        END_IF
    END_LOOP
    // It rushes at the target at high speed.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HANNYA_MOVE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    // Find out how the target reacts to the charge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_hannya_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // The charge missed: it keeps rushing past the target.
    LABEL(90)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    MULTIPLY(LW(14), 500)
    ADD(LW(0), LW(14))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    GOTO(98)
    // The charge connects: the damage is applied and it carries on past the target.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_hannya_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_hannya_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    MULTIPLY(LW(14), 500)
    ADD(LW(0), LW(14))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    GOTO(98)
    // It turns around and drifts back to its home position, slowing down as it arrives.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HANNYA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 300, LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 50)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HANNYA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 30)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 10)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_MS(300)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait event: sets the idle animation from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

