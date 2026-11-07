// Pennington (`powan`): a tst-only test unit, a c_kinopio rider (part 1) on a c_gazigazi body (parts 2 and 3). After each won
// battle the field battle's post-battle event respawns it, four times in all, with a new line each time.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "system.h"
#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "driver/animdrv.h"
#include "driver/npcdrv.h"
#include "manager/evtmgr_cmd.h"
#include "tst.h"
#include "battle/battle_sub.h"
#include "evt/evt_msg.h"

// Unit work slots of Powan (btlevtcmd_SetUnitWork / GetUnitWork).
enum {
    UW_POWAN_RESPAWN_COUNT = 0, // life number (respawns use 2 through 5); picks its complaint and last line
};

static USER_FUNC(powan_set_post_battle_mode);
static USER_FUNC(powan_set_post_battle_event);
static USER_FUNC(powan_rider_follow);

BattleUnitKind unit_powan;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table_kinopio[5];
static PoseTableEntry pose_table_gazigazi[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[3];
static EvtWord init_event[26];
static EvtWord rider_follow_event[21];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static BattleWeapon weapon_powan_attack;
static EvtWord attack_event[193];
static EvtWord hit_event[154];
static EvtWord wait_event[6];
static BattleUnitSetup entry_powan;
static EvtWord post_battle_event_2[13];
static EvtWord post_battle_event_3[13];
static EvtWord post_battle_event_4[13];
static EvtWord post_battle_event_5[13];
static EvtWord respawn_event[111];
static EvtWord dead_event[116];

/**
 * Unit definition of Pennington: 1 HP and level 1, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_powan = {
    209,
    "btl_un_powan",
    1,
    0,
    1,
    1,
    1,
    0,
    0,
    100,
    0,
    100,
    9,
    24,
    24,
    24,
    24,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    0.0f,
    12.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BTL_DAMAGE1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    3,
    parts,
    init_event,
    data_table,
};

/** Defence of Pennington: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of Pennington: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Status vulnerability of Pennington: immune to fright; extra prone to nearly every status. */
static StatusVulnerability regist = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 0, 150, 150, 150, 150, 150, 150};

/** Pose table of the rider on Pennington (part 1): maps each battle pose to its KNP_* animations. */
static PoseTableEntry pose_table_kinopio[] = {
    {POSE_TYPE_NORMAL, "KNP_S_1"},
    {31, "KNP_S_1"},
    {POSE_TYPE_DAMAGE, "KNP_O_1"},
    {POSE_TYPE_RUN, "KNP_R_1"},
    {POSE_TYPE_ACTION, "KNP_S_1"},
};

/**
 * Pose table of the body Pennington rides on (parts 2 and 3): maps each battle pose to its GAZ_* animations.
 */
static PoseTableEntry pose_table_gazigazi[] = {
    {POSE_TYPE_NORMAL, "GAZ_S_1"},
    {31, "GAZ_A_1"},
    {POSE_TYPE_DAMAGE, "GAZ_A_1"},
    {POSE_TYPE_ACTION, "GAZ_S_1"},
};

/** Data table of Pennington: reaction scripts for being defeated. */
static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

/**
 * Parts of Pennington: the rider (part 1, model c_kinopio)
 * and the two-piece body he sits on (parts 2 and 3, model c_gazigazi).
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_powan", "c_kinopio", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_kinopio},
    {2, "btl_un_powan", "c_gazigazi", 0.0f, 10.0f, 6.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table_gazigazi},
    {3, "btl_un_powan", "c_gazigazi", 0.0f, 10.0f, 6.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_gazigazi},
};

/**
 * Battle init script for the test unit Powan: installs the wait,
 * attack and damage handlers (confusion uses the default one),
 * registers its post-battle event and starts rider_follow_event to update body part 2 from its animation joint.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(powan_set_post_battle_event, PTR(post_battle_event_2))
    SPAWN_EVENT(rider_follow_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Looping helper started from init_event: every frame, while the unit exists,
 * calls powan_rider_follow to update body part 2 from its own pPlane1 animation joint.
 */
static EVT_BEGIN(rider_follow_event)
    LABEL(0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(powan_rider_follow, UNIT_ID_SELF, 2)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Damage handler: selects this unit (-2) and its part 1
 * and runs the shared default damage reaction (btldefaultevt_Damage).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Empty phase handler. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Attack data of Pennington's attack: 3 damage. */
static BattleWeapon weapon_powan_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    4, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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
 * Attack of the test unit Powan: uses an enemy item if the item-use check selects one, otherwise picks a sampled enemy,
 * walks up to it, lowers the body and swings (GAZ_A_1) for an attack that is always resolved at once (the
 * pre-damage check is followed directly by the defence and damage calls), then walks home.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetCounterAcCallBack, UNIT_ID_SELF, PTR(hit_event))
    SET_VALUE(LW(12), PTR(&weapon_powan_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("GAZ_A_1"))
    WAIT_FRAMES(12)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, -2, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * When the attack is Superguarded the body is knocked away from under the rider: part 2 is hidden and its copy
 * (part 3) flies spinning to the stage edge; the rider runs after it, the body is shown on part 2 again and the
 * rider returns home.
 */
static EVT_BEGIN(hit_event)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    ADD(LW(2), 6)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KNP_O_1"))
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 3, FLOAT(0.07f))
    BEGIN_SIBLING_EVENT
        LOOP(90)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 3, 0, 40, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 90, -1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    MULTIPLY(LW(5), -1)
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    MULTIPLY(LW(5), -1)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Wait (idle) event: refreshes the body animation (part 1) from the unit's pose table
 * so it matches the current state.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Setup the field battle's post-battle event uses to respawn Pennington for the next battle:
 * enemy side, placed far off-screen until the script moves it into position.
 */
static BattleUnitSetup entry_powan = {
    &unit_powan, // unitKindParams
    1, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Post-battle events: each respawns the unit with the next counter value (unit work 0) and arms the next one. */
static EVT_BEGIN(post_battle_event_2)
    SET_VALUE(LW(5), 2)
    CALL_CHILD(respawn_event)
    CALL(powan_set_post_battle_event, PTR(post_battle_event_3))
    CALL(powan_set_post_battle_mode, 1)
    RETURN
EVT_END()

/**
 * Post-battle event for the counter-3 respawn: respawns Powan with counter 3 (respawn_event),
 * then registers post_battle_event_4 to run after the next battle.
 */
static EVT_BEGIN(post_battle_event_3)
    SET_VALUE(LW(5), 3)
    CALL_CHILD(respawn_event)
    CALL(powan_set_post_battle_event, PTR(post_battle_event_4))
    CALL(powan_set_post_battle_mode, 1)
    RETURN
EVT_END()

/**
 * Post-battle event for the counter-4 respawn: respawns Powan with counter 4,
 * then registers post_battle_event_5 to run after the next battle.
 */
static EVT_BEGIN(post_battle_event_4)
    SET_VALUE(LW(5), 4)
    CALL_CHILD(respawn_event)
    CALL(powan_set_post_battle_event, PTR(post_battle_event_5))
    CALL(powan_set_post_battle_mode, 1)
    RETURN
EVT_END()

/** Post-battle event for the last respawn: respawns Powan with counter 5 and registers no further post-battle event. */
static EVT_BEGIN(post_battle_event_5)
    SET_VALUE(LW(5), 5)
    CALL_CHILD(respawn_event)
    CALL(powan_set_post_battle_event, 0)
    CALL(powan_set_post_battle_mode, 1)
    RETURN
EVT_END()

/**
 * Respawns Powan after a battle (LW(5) is the respawn counter, stored in its unit work 0):
 * spawns entry_powan high above the stage at x = 80,
 * lets it fall to the floor and then prints a Japanese complaint from it that depends on the counter (the lines
 * get more worn out each time).
 */
static EVT_BEGIN(respawn_event)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_powan), 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), UW_POWAN_RESPAWN_COUNT, LW(5))
    SET_VALUE(LW(0), 80)
    SET_VALUE(LW(1), 240)
    SET_VALUE(LW(2), 0)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(3), FLOAT(0.1f))
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), LW(1), LW(2), 120, -1)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    WAIT_FRAMES(30)
    SWITCH(LW(5))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 3, STRING("あいたた<key><scrl auto> \nひどいじゃないか\nでもいまのは　カゲムシャ<key>"), 0, LW(3))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 3, STRING("あいたた<key>"), 0, LW(3))
        CALL(evt_msg_print, 3, STRING("もしかして<key>"), 0, LW(3))
        CALL(evt_msg_print, 3, STRING("このまえきみを\nはんにんあつかいしたことを\nうらんでいるのかね<key>"), 0, LW(3))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 3, STRING("あいたた<key>"), 0, LW(3))
        CALL(evt_msg_print, 3, STRING("もうそろそろやばいって\nしゃれにならんて<key>"), 0, LW(3))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 3, STRING("ぐふっ<key>"), 0, LW(3))
        CALL(evt_msg_print, 3, STRING("もうこれいじょうはあかんて。\nいや、ほんまに。<key>"), 0, LW(3))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Death script of Powan: on the fifth life it prints a last line first.
 * It then spins 15 degrees per frame for 48 frames, banks its coins and experience,
 * tips over for 30 frames and uses kill mode 0 if DAMAGE_RESULT_DEATH is set, otherwise mode 2 (marks the unit inactive).
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(10))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_POWAN_RESPAWN_COUNT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(evt_msg_print, 3, STRING("もう\nからだもたんわー<key>"), 0, UNIT_ID_SELF)
    END_IF
    SET_VALUE(LW(0), 48)
    LABEL(10)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

/** arg 0: FieldBattleData.postBattleMode */
static USER_FUNC(powan_set_post_battle_mode) {
    s32 mode = evtGetValue(event, event->args[0]);

    fbatGetPointer()->postBattleMode = mode;
    return EVT_RETURN_DONE;
}

/** arg 0: the event FieldBattleData runs after a won battle */
static USER_FUNC(powan_set_post_battle_event) {
    void* evt = (void*)evtGetValue(event, event->args[0]);

    fbatGetPointer()->postBattleEvent = evt;
    return EVT_RETURN_DONE;
}

/** args: unit id, part id. Updates the requested part's position from the "pPlane1" joint of that same part's animation. */
static USER_FUNC(powan_rider_follow) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 partId = evtGetValue(event, args[1]);
    BattleUnitPart* part;
    BattleWork* wp = _battleWorkPointer;
    const char* joint = "pPlane1";
    AnimationPose* pose;
    s32 idx;
    f32 x;
    f32 y;
    f32 z;

    part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(wp, BattleTransID(event, id)), partId);
    if (part->poseId == -1) {
        return EVT_RETURN_DONE;
    }
    idx = animPoseGetGroupIdx(part->poseId, joint);
    pose = animPoseGetAnimPosePtr(part->poseId);
    if (pose->worldPositions == NULL) {
        animPoseWorldPositionEvalOn(part->poseId);
        x = 0.0f;
        y = -1000.0f;
        z = 0.0f;
    } else {
        x = (*(Vec**)pose->worldPositions)[idx].x; // FAKE: smartEntry viewed as its address word; ->address with a cast loads differently
        y = (*(Vec**)pose->worldPositions)[idx].y;
        z = (*(Vec**)pose->worldPositions)[idx].z;
    }
    BtlUnit_SetPartsPos(part, x, y, z);
#if VERSION >= VERSION_JP // the demos do not print the position
    sysOSReport("unit_powan.c", 275, "x:%f y:%f z:%f\n", x, y, z);
#endif
    return EVT_RETURN_DONE;
}
