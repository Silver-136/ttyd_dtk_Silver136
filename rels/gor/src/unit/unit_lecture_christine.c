// unit_lecture_christine: Goombella as she appears in the action-command tutorial battle.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/unit_work.h"

#include "battle/battle.h"
#include "battle/battle_pad.h"
#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_christine.h"
#include "battle/battle_database_common.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_PAIRED_UNIT_UNUSED 0 // the lecture_frankli unit's id, written at setup and never read (Frankly reads his own slot 0, which holds this unit's id)

static USER_FUNC(_battle_set_lecture_unit_id);
static USER_FUNC(_set_lecture_mode);
static USER_FUNC(_get_defence_ac_result);
static USER_FUNC(_config_key_record);
static USER_FUNC(_krb_get_dir);
extern BattleUnitKind unit_lecture_frankli;

BattleUnitKind unit_lecture_christine;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleUnitSetup entry_lecture_frankli;
static EvtWord init_event[78];
static EvtWord damage_event[10];
static EvtWord lecture_christine_ac_counter_damage_event[117];
static EvtWord phase_event[2];
static EvtWord wait_event[6];
static EvtWord attack_event[19];
static BattleWeapon lec_weapon_zutsuki_1;
static BattleWeapon lec_weapon_zutsuki_2;

#if VERSION == VERSION_DEMO_JP
static EvtWord lecture_zutsuki_attack_event[554];
#else
static EvtWord lecture_zutsuki_attack_event[570];
#endif

/**
 * Script helper that converts the defence action command result of the battle into the tutorial's own code:
 * 2 for result 1, 3 for 2, 1 for 4, 0 for 5, otherwise 0, stored in args[0].
 */
static USER_FUNC(_get_defence_ac_result) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 var = args[0];
    s32 result = 0;

    switch (wp->actionCommands.defenseResult) {
        case 1:
            result = 2;
            break;
        case 2:
            result = 3;
            break;
        case 4:
            result = 1;
            break;
        case 5:
            result = 0;
            break;
    }
    evtSetValue(event, var, result);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that fakes a recorded button press for the tutorial:
 * marks the defence result as 4, resets the action command pad
 * and stores args[0] as the first entry of the pad's pressed-button history.
 */
static USER_FUNC(_config_key_record) {
    s32 record = event->args[0];
    BattleWork* wp = _battleWorkPointer;
    BattleWorkPad* pad = &wp->actionCommands.pad;

    wp->actionCommands.defenseResult = 4;
    BtlPad_WorkInit(pad);
    pad->buttonsPressedHistory[0] = record;
    return EVT_RETURN_DONE;
}

/**
 * Unit kind of Goombella:
 * 10 HP, level 1, with its size, sounds and the links to its parts, status vulnerability and scripts.
 */
BattleUnitKind unit_lecture_christine = {
    UNIT_TUTORIAL_GOOMBELLA, // unitType
    "btl_un_christine", // name
    10, // maxHp
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
    28, // width
    24, // height
    {8, 24}, // hitOffset
    {0.0f, 14.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 30.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 15.6f, 0.0f}, // kissHitOffset
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
    "SFX_PARTY_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoHpGauge, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of Goombella's body per damage type: 0 against every damage type. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of Goombella's body: none, so every damage type is dealt normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Status vulnerability of Goombella: immune to the 22 statuses covered by this table. */
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

/**
 * Event table of Goombella in the tutorial battle: tag 36 is her reaction when she is hurt while attacking.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, lecture_christine_ac_counter_damage_event},
    {0, NULL},
};

/** Body part of Goombella: a single part using the model "c_pkr". */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_christine", "c_pkr", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kUntattleable | kNoHpLoss, 0, pose_table_christine_stay},
};

/** Setup entry that adds Prof. Frankly at (0, 0, -45) when the tutorial battle starts. */
static BattleUnitSetup entry_lecture_frankli = {
    &unit_lecture_frankli, // unitKindParams
    ALLIANCE_NEUTRAL, // alliance
    {0}, // pad_05
    BATTLE_PHASE_BEFORE_PLAYER, // attackPhase
    {0.0f, 0.0f, -45.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Init event of Goombella in the tutorial battle:
 * installs phase, attack, damage and confusion events and her partner walk, run and jump sounds,
 * switches the battle to lecture mode, spawns Professor Frankly (entry_lecture_frankli,
 * stored as work 0 and registered as the lecture unit) facing left and gives him her unit id.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(_set_lecture_mode)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_lecture_frankli), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PAIRED_UNIT_UNUSED, LW(3))
    CALL(_battle_set_lecture_unit_id, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), -1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), LECTURE_FRANKLI_WORK_CHRISTINE, LW(0))
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_NO_OBJECT_FALL)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of Goombella in the tutorial battle: runs the default damage handling. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Counter-damage event (data table tag 36) of Goombella in the tutorial's Superguard lesson, run when Mario's
 * Superguard turns her Headbonk back on her: she is knocked into the air, lands, hops twice,
 * says mac_0_b_t2_015 and advances the lecture's unit work (to 8 if it was 7,
 * otherwise sets 7 on the other slot).
 */
static EVT_BEGIN(lecture_christine_ac_counter_damage_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 5, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("PKR_D_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("PKR_D_1"))
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("mac_0_b_t2_015"), 0, -2)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(2), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(2), 15)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 10, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 7)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, 8)
    ELSE
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_GUARD_RESULT, 7)
    END_IF
    RETURN
EVT_END()

/**
 * Phase event of Goombella in the tutorial battle:
 * does nothing (Professor Frankly's phase event drives the lesson).
 */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Wait (idle) event of Goombella in the tutorial battle: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Attack event of Goombella in the tutorial battle:
 * runs the scripted headbonk lesson (lecture_zutsuki_attack_event),
 * then advances the lecture's work 1 from 3 to 4.
 */
static EVT_BEGIN(attack_event)
    CALL_CHILD(lecture_zutsuki_attack_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, 4)
    END_SWITCH
    RETURN
EVT_END()

/** Goombella's headbutt 1: 1 damage to a single target, a jump-style hit. */
static BattleWeapon lec_weapon_zutsuki_1 = {
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
    weaponGetPowerDefault, // hpDamageFn
    {1, 1, 1, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Goombella's headbutt 2: 1 damage to a single target, a jump-style hit. */
static BattleWeapon lec_weapon_zutsuki_2 = {
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
    {1, 1, 1, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Goombella's Headbonk (ずつき) in the guard lessons: picks lec_weapon_zutsuki_1 at lecture progress 3, else
 * lec_weapon_zutsuki_2, and jumps at Mario.
 * Mid-jump, Professor Frankly freezes the action: at progress 3 for the
 * A guard (mac_0_b_t2_006), at progress 7 for the B Superguard (mac_0_b_t2_014), and waits for the button. After
 * the hit it stores the guard result in the lecture's unit work 3 (4, 5 or 6) and returns home.
 */
static EVT_BEGIN(lecture_zutsuki_attack_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 3)
        SET_VALUE(LW(12), PTR(&lec_weapon_zutsuki_1))
    ELSE
        SET_VALUE(LW(12), PTR(&lec_weapon_zutsuki_2))
    END_IF
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: Mario, for the guard lesson.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // She runs to a spot in front of Mario.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(14))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(14), MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(10), 0)

    // Wind-up: she crouches and jumps at him with a Goomba jump cry.
    LABEL(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_A_2B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKR_J_1A"))
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(15)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_KURI_ATTACK1"), 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 14, 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        SET_VALUE(LW(8), 0)
        LOOP(36)
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(12), LW(13), LW(14))
            CALL(_krb_get_dir, LW(5), LW(6), LW(12), LW(13), LW(8))
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, LW(0), LW(1), LW(8))
            SET_FLOAT_VALUE(LW(5), LW(12))
            SET_FLOAT_VALUE(LW(6), LW(13))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)

    // Mid-jump, Professor Frankly freezes the action and explains how to guard.
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetTalkPose, UNIT_ID_LECTURE, STRING("T_1"))
        CALL(btlevtcmd_SetStayPose, UNIT_ID_LECTURE, STRING("S_1"))

        // Guard lesson: he asks Mario to press the A button as she lands, and waits for it.
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("mac_0_b_t2_006"), 0, UNIT_ID_LECTURE)
#else
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("mac_0_b_t2_006"), 0, -6)
        WAIT_FRAMES(1)
#endif
        LABEL(11)
        CALL(btlevtcmd_PadCheckTrigger, 0, PAD_BUTTON_A, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(11)
        END_IF
#if VERSION >= VERSION_JP
        WAIT_MS(50)
#endif
        BEGIN_INLINE_EVENT
#if VERSION >= VERSION_JP
            WAIT_MS(100)
#endif
            CALL(evt_msg_continue)
        END_INLINE_EVENT
#if VERSION >= VERSION_JP
        WAIT_MS(100)
#endif
        CASE_EQUAL(7)
        CALL(btlevtcmd_SetTalkPose, UNIT_ID_LECTURE, STRING("T_1"))
        CALL(btlevtcmd_SetStayPose, UNIT_ID_LECTURE, STRING("S_1"))

        // Superguard lesson: he asks for the B button instead, and waits for it.
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("mac_0_b_t2_014"), 0, UNIT_ID_LECTURE)
#else
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("mac_0_b_t2_014"), 0, -6)
        WAIT_FRAMES(1)
#endif
        LABEL(12)
        CALL(btlevtcmd_PadCheckTrigger, 0, PAD_BUTTON_B, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(12)
        END_IF
#if VERSION >= VERSION_JP
        WAIT_MS(50)
#endif
        BEGIN_INLINE_EVENT
#if VERSION >= VERSION_JP
            WAIT_MS(100)
#endif
            CALL(evt_msg_continue)
        END_INLINE_EVENT
#if VERSION >= VERSION_JP
        WAIT_MS(100)
#endif
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
        CALL(evt_btl_camera_set_zoom, 0, FLOAT(162.5f))
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(3)

        // The pressed buttons are recorded for the lecture.
        CALL(_config_key_record, PAD_BUTTON_A)
        CASE_EQUAL(7)
        CALL(_config_key_record, PAD_BUTTON_B)
    END_SWITCH

    // The hit is resolved with the lesson's guard result and stored for Frankly.
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(_get_defence_ac_result, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_GUARD_RESULT, 4)
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_GUARD_RESULT, 5)
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_FRANKLI_WORK_GUARD_RESULT, 6)
    END_SWITCH
    ADD(LW(10), 1)

    // She bounces off Mario in three small hops.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        IF_INT_LESS(LW(6), 0)
            MULTIPLY(LW(6), -1)
        END_IF
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(6), LW(0))
        DIVIDE(LW(6), 15)
        SET_VALUE(LW(7), 15)
        LABEL(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)

    // She walks back to her home position.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Script helper: computes the facing direction (degrees) from point (args[0], args[1]) to (args[2], args[3])
 * and stores it in args[4]; if both points coincide args[4] is left as it was.
 */
static USER_FUNC(_krb_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 y1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 y2 = evtGetFloat(event, args[3]);
    s32 dir = evtGetValue(event, args[4]);
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;

    if (dx == 0.0f && dy == 0.0f) {
        evtSetValue(event, args[4], dir);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[4], angleABf(dx, dy, 0.0f, 0.0f));
    return EVT_RETURN_DONE;
}

/** Script helper that sets the battle's lecture unit id to args[0], the unit that the tutorial messages come from. */
static USER_FUNC(_battle_set_lecture_unit_id) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 value;

    value = evtGetValue(event, *args++);
    wp->lectureUnitId = value;
    return EVT_RETURN_DONE;
}

/**
 * Script helper that turns on the lecture mode of the battle:
 * locks Mario's HP and sets lectureMode.
 */
static USER_FUNC(_set_lecture_mode) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = BattleTransID(event, UNIT_ID_MARIO);
    BattleUnit* unit = BattleGetUnitPtr(wp, id);

    unit->attributeFlags |= kUnitHpFixed;
    wp->lectureMode = 1;
    return EVT_RETURN_DONE;
}
