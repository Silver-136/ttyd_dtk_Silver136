// Battle unit Hyper Goomba (ハイパークリボー): gra.rel's copy.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"

#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_CHARGED_UNUSED 0 // Records charge use; attack selection checks STATUS_CHARGE instead.

static USER_FUNC(krb_get_dir);

BattleUnitKind unit_hyper_kuriboo;
static PoseTableEntry pose_table[20];
static BattleDataEntry data_table[2];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_kuriboo_attack;
static BattleWeapon weapon_kuriboo_charge;
static EvtWord init_event[47];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[638];
static EvtWord wait_event[6];

/**
 * Unit kind of the Hyper Goomba:
 * 8 HP, level 15, with its size, sounds and the links to its parts, status vulnerability and scripts.
 */
BattleUnitKind unit_hyper_kuriboo = {
    UNIT_HYPER_GOOMBA, // unitType
    "btl_un_hyper_kuriboo", // name
    8, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    15, // level
    0, // bonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    70, // runRate
    9999, // pbSoftCap
    23, // width
    25, // height
    {5, 25}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {12.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {11.5f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 18.0f, 0.0f}, // kissHitOffset
    {0.0f, 12.0f, 0.0f}, // cutBaseOffset
    23.0f, // cutWidth
    25.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_KURI_DAMAGED1", // damageSfxName
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
 * Pose table of the Hyper Goomba:
 * the animation played for each pose type (idle, hurt, walk, run, attack and so on).
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KUR_N_1"},
    {POSE_TYPE_STOP, "KUR_Y_1"},
    {POSE_TYPE_FREEZE, "KUR_Y_1"},
    {POSE_TYPE_CONFUSE, "KUR_K_1"},
    {POSE_TYPE_POISON, "KUR_I_1"},
    {POSE_TYPE_DIZZY, "KUR_I_1"},
    {POSE_TYPE_NORMAL, "KUR_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KUR_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KUR_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KUR_S_1"},
    {POSE_TYPE_DAMAGE, "KUR_D_1"},
    {POSE_TYPE_ATTACK, "KUR_A_1"},
    {POSE_TYPE_ATTACK_2, "KUR_A_2"},
    {POSE_TYPE_RUN, "KUR_R_1"},
    {POSE_TYPE_RUN_HOME, "KUR_R_2"},
    {POSE_TYPE_WALK, "KUR_W_1"},
    {POSE_TYPE_DANGER, "KUR_I_1"},
    {POSE_TYPE_PERIL, "KUR_I_1"},
    {POSE_TYPE_TALK, "KUR_T_1"},
    {POSE_TYPE_ACTION, "KUR_S_1"},
};

/** Event table of the Hyper Goomba: nothing special on a First Strike. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Defence of the Hyper Goomba's body per damage type: 0 against every damage type. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of the Hyper Goomba's body: none, so every damage type is dealt normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {80, 100, 80, 100, 80, 100, 100, 80, 100, 80, 100, 80, 100, 80, 80, 100, 100, 100, 80, 100, 100, 95};
#else
static StatusVulnerability regist = {
    80, // sleep
    90, // stop
    80, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    80, // freeze
    100, // huge
    95, // tiny
    100, // attackUp
    95, // attackDown
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

/** Body part of the Hyper Goomba: a single part using the model "c_kuribo_h". */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_hyper_kuriboo", "c_kuribo_h", 0.0f, 0.0f, 0.0f, 0.0f, 22.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Hyper Goomba's attack: 2 damage to a single target, a jump-style hit. */
static BattleWeapon weapon_kuriboo_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
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

/** Hyper Goomba's charge-up move (no damage, used on itself). */
static BattleWeapon weapon_kuriboo_charge = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    6, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Init event of the Hyper Goomba: installs wait, attack, damage and confusion events, sets the Goomba run,
 * walk and jump sounds and clears unit work 0 (the "charged" flag).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGED_UNUSED, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of the Hyper Goomba: runs the default damage handling. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/** Phase event of the Hyper Goomba: does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/**
 * Attack event of the Hyper Goomba: if it is not already charged (STATUS_CHARGE) it may use an enemy item;
 * otherwise it draws lots (25% on turn 1, 25/25 later) to either spend its turn charging,
 * with a crystal effect and a hop (weapon_kuriboo_charge, unit work 0 set to 1),
 * or to run at a target and headbutt it (weapon_kuriboo_attack),
 * with the pre-damage check choosing between a dodge and a hit.
 */
static EVT_BEGIN(attack_event)
    // Target: the first-strike target if the battle opened that way, otherwise one picked for its attack.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_kuriboo_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_kuriboo_attack), LW(3), LW(4))
    END_IF

    // No target: a confused one flusters, otherwise the attack ends.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // Already charged: it goes straight to the headbutt.
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CHARGE, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(10)
    END_IF

    // It may use an enemy item instead of acting.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // It draws lots between spending the turn charging up and attacking.
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_DrawLots, LW(0), 2, 25, 75)
    ELSE
        CALL(btlevtcmd_DrawLots, LW(0), 2, 25, 25)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_kuriboo_charge))
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGED_UNUSED, 1)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
        CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 300)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 15)
        ADD(LW(2), 10)

        // Charge-up: the charge sound and a crystal effect play while it hops in place.
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_eff64, STRING(""), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1000)
        BEGIN_INLINE_EVENT
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, 0, FLOAT(1.0f))
            CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
            WAIT_MS(500)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        END_INLINE_EVENT

        // The charge weapon applies the charged state to itself and the turn ends.
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF

    // Headbutt: the attack is declared.
    LABEL(10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_kuriboo_attack))

    // It runs up to the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // Wind-up: a short pause, then it leaps at the target, spinning in the air.
    WAIT_MS(170)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        SET_VALUE(LW(8), 0)
        LOOP(32)
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(9), LW(10), LW(11))
            CALL(krb_get_dir, LW(5), LW(6), LW(9), LW(10), LW(8))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(8))
            SET_FLOAT_VALUE(LW(5), LW(9))
            SET_FLOAT_VALUE(LW(6), LW(10))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(12)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(15), 50)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))

    // The leap comes down on the target; the pre-damage check decides between a dodge and a hit.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 32, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kuriboo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
        GOTO(91)
        CASE_END
    END_SWITCH

    // Miss: it hops on past the target, which dodges for some kinds of miss.
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(15), POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(500)
    GOTO(95)

    // Hit: the attack lands for its damage and the Goomba bounces back off the target.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kuriboo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(15), POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
    GOTO(95)

    // It walks back to its home position.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 3, 3, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN_HOME)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait (idle) event of the Hyper Goomba: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Script helper: computes the facing direction (degrees) from point (args[0], args[1]) to (args[2], args[3])
 * and stores it in args[4]; if both points coincide args[4] is left as it was.
 */
static USER_FUNC(krb_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 z1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 z2 = evtGetFloat(event, args[3]);
    s32 dir = evtGetValue(event, args[4]);
    f32 dx = x2 - x1;
    f32 dz = z2 - z1;

    if (dx == 0.0f && dz == 0.0f) {
        evtSetValue(event, args[4], dir);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[4], angleABf(dx, dz, 0.0f, 0.0f));
    return EVT_RETURN_DONE;
}
