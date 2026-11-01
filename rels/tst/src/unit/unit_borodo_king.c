// tst.rel's version of rels/tou2/src/unit/unit_borodo_king.c (Big Bandit), the same earlier version as
// tst's unit_borodo.c.
// The JP demo's copy has none of the debug prints or the assert.
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "mario/mario_pouch.h"
#include "data/item_data.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "battle/battle_icon.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "driver/msgdrv.h"
#include "tst_battle.h"
#include "tst_debug.h"
#include "version.h"

static USER_FUNC(_steal_item);

BattleUnitKind unit_borodo_king;
static BattleWeapon weapon_borodo_attack;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[20];
static PoseTableEntry pose_table_item_have[18];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[57];
static EvtWord damage_event[10];
static EvtWord attack_event[59];
static EvtWord _borodo_normal_attack[503];
static EvtWord _borodo_steal_attack[610];
static EvtWord escape_event[115];
static EvtWord wait_event[26];

/**
 * Unit kind of Big Bandit: 8 max HP, level 13. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_borodo_king = {
    45,
    "btl_un_borodo_king",
    8,
    0,
    1,
    1,
    13,
    0,
    3,
    70,
    1,
    50,
    9999,
    26,
    34,
    0,
    34,
    0.0f,
    17.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    0.0f,
    9.0f,
    26.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    26.0f,
    34.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_BORODO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/** Attack data of Big Bandit's borodo attack: base damage 3, 100% accuracy. */
static BattleWeapon weapon_borodo_attack = {
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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
 * Defence of Big Bandit per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Big Bandit: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of Big Bandit (the demos), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 60, Stop 80, Dizzy 80, Confuse 80, Freeze 60, Tiny 80, Attack Down 80, Defense Down 80,
 * Allergic 80, Fright 85, Gale Force 70, Slow 80 and instant KO 95.
 */
static StatusVulnerability regist = {60, 80, 80, 100, 80, 100, 100, 60, 100, 80, 100, 80, 100, 80, 80, 85, 70, 100, 80, 100, 100, 95};
#else
/**
 * Status vulnerability of Big Bandit (the final versions), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 90, Stop 80, Dizzy 60, Confuse 90, Freeze 60, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 80, Fright 85, Gale Force 70, Slow 80 and instant KO 95.
 */
static StatusVulnerability regist = {90, 80, 60, 100, 90, 100, 100, 60, 100, 90, 100, 90, 100, 95, 80, 85, 70, 100, 80, 100, 100, 95};
#endif

/**
 * Pose table of Big Bandit: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 22 entries such as N_1, Y_1, K_1, X_1.
 */
static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "X_1"},
    {BTL_POSE_NORMAL, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "S_1"},
    {BTL_POSE_DAMAGE, "D_1"},
    {50, "A_1"},
    {51, "A_2"},
    {POSE_TYPE_RUN, "R_1"},
    {41, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
#if VERSION <= VERSION_DEMO_US
    {POSE_TYPE_DANGER, "I_1"},
    {POSE_TYPE_PERIL, "I_1"},
#else
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
#endif
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/**
 * Pose table of Big Bandit while it holds a stolen item: maps its battle pose types (stay, walk, hurt,
 * dead, talk and so on) to animation names, 20 entries such as N_1, Y_1, K_1, I_1.
 */
static PoseTableEntry pose_table_item_have[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
#if VERSION <= VERSION_DEMO_US
    {4, "I_1"},
    {3, "I_1"},
#else
    {4, "X_1"},
    {3, "X_1"},
#endif
    {BTL_POSE_NORMAL, "S_2"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "S_2"},
    {BTL_POSE_DAMAGE, "D_1"},
    {50, "A_1"},
    {51, "A_2"},
    {POSE_TYPE_RUN, "R_2"},
    {41, "R_2"},
    {POSE_TYPE_WALK, "W_2"},
    {POSE_TYPE_TALK, "S_2"},
    {POSE_TYPE_ACTION, "S_2"},
};

/**
 * Event table of Big Bandit: maps event tags to the scripts that react to them (no custom reactions);
 * tag 0 ends it.
 */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Body parts of Big Bandit: a single part (model c_borodo_g) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_borodo_king", "c_borodo_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/**
 * Init script of the Bandit: registers its wait, attack, damage and confusion handlers,
 * sets the running and walking sounds, and picks the pose table with the stolen-item bag
 * when the unit flag 0x40000000 (it carries a stolen item) is already set.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), STRING("SFX_ENM_BORODO_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), STRING("SFX_ENM_BORODO_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_CheckUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_item_have))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage reaction: runs the default damage handling (btldefaultevt_Damage) with LW(10) = -2 (the unit itself)
 * and LW(11) = 1.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Phase event: empty, it does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/**
 * Chooses the Big Bandit's action: if it carries a stolen item (unit flag 0x40000000) it runs away with it
 * (escape_event); otherwise it uses an item if it has one, a confused Bandit makes a plain attack,
 * and normally it makes the plain attack (_borodo_normal_attack) about 35% of the time
 * and the stealing attack (_borodo_steal_attack) otherwise.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_CheckUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(escape_event)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(_borodo_normal_attack)
        RETURN
    END_IF
    SET_VALUE(LW(1), 215)
    SUBTRACT(LW(1), 1)
    CALL(evt_sub_random, LW(1), LW(0))
    IF_INT_LESS(LW(0), 75)
        CALL_CHILD(_borodo_normal_attack)
    ELSE
        CALL_CHILD(_borodo_steal_attack)
    END_IF
    RETURN
EVT_END()

/**
 * The Bandit's plain attack: it runs up to the target (SFX_ENM_BORODO_MOVE1L/R),
 * bumps it (SFX_ENM_BORODO_ATT1) and either misses or applies the damage with the action-command defence;
 * afterwards it falls over or recovers and runs back home.
 * A special pre-damage result of 5 skips the miss handling.
 */
static EVT_BEGIN(_borodo_normal_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    // It picks a target; with none, it ends the turn (or flusters when confused).
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_borodo_attack), LW(3), LW(4))
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
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_borodo_attack))
    // The attack is declared and paid for, and the camera follows the Big Bandit.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_borodo_attack))
    // It runs up to the target, its feet pattering.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    LOOP(8)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
    END_LOOP
    CALL(evt_btl_camera_set_mode, 0, 3)
    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
#if VERSION <= VERSION_DEMO_US
        MULTIPLY(LW(6), 40)
#else
        MULTIPLY_FLOAT(LW(6), 40)
#endif
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    // It bumps into the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_snd_sfxoff, LW(14))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    // On a miss it hops back twice, thrown off balance.
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    GOTO(98)
    // On a hit the action-command defence is resolved and the damage dealt; the Big Bandit reels from the bump and ends up dazed for a second.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.25f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 40)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(5), 0)
            SET_VALUE(LW(6), 360)
        ELSE
            SET_VALUE(LW(5), 360)
            SET_VALUE(LW(6), 0)
        END_IF
        CALL(btlevtcmd_SetRotateOffsetFromCenterOffset, UNIT_ID_SELF)
        CALL(evt_sub_intpl_init, 13, LW(5), LW(6), 24)
        LOOP(25)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("X_1"))
    WAIT_MS(1000)
    // The camera resets and the Big Bandit runs back home.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Bandit's stealing attack: it runs at the target and bumps it like the plain attack;
 * on a hit where Mario's action command result (LW(7)) was 3 or less,
 * _steal_item takes an item from Mario's pouch (LW(8) = the item, LW(9) = its icon),
 * which flies out of the target while btl_msg_steal_item_lost is announced and sets the unit flag 0x40000000,
 * and the Bandit laughs (SFX_ENM_BORODO_LAUGH1) and switches to the pose table showing the loot.
 * If nothing was stolen it sulks (SFX_ENM_BORODO_INCLINE1) and walks home.
 */
static EVT_BEGIN(_borodo_steal_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    // It picks a target; with none, it ends the turn (or flusters when confused).
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_borodo_attack), LW(3), LW(4))
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
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_borodo_attack))
    // The attack is declared and paid for, and the camera follows the Big Bandit.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_borodo_attack))
    // It runs up to the target, its feet pattering.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    LOOP(8)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
    END_LOOP
    CALL(evt_btl_camera_set_mode, 0, 3)
    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
#if VERSION <= VERSION_DEMO_US
        MULTIPLY(LW(6), 40)
#else
        MULTIPLY_FLOAT(LW(6), 40)
#endif
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    // It bumps into the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_snd_sfxoff, LW(14))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    // On a miss it hops back twice, thrown off balance.
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    GOTO(98)
    // On a hit the action-command defence is resolved and the damage dealt.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SET_VALUE(LW(8), 0)
    CALL(btlevtcmd_GetResultACDefence, LW(7))
    IF_INT_LESS_OR_EQUAL(LW(7), 3)
        CALL(_steal_item, UNIT_ID_SELF, LW(3), LW(8), LW(9), LW(7))
        IF_INT_NOT_EQUAL(LW(8), 0)
            // If Mario's action command was poor enough, the Big Bandit snatches an item; it flies out of the target in an arc.
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_SUCCESS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            BEGIN_INLINE_EVENT
                WAIT_FRAMES(20)
                CALL(btlevtcmd_AnnounceSetParam, 0, LW(8))
                CALL(btlevtcmd_AnnounceMessage, 1, 0, 0, STRING("btl_msg_steal_item_lost"), 90)
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_BtlIconEntry, LW(9), LW(0), LW(1), LW(2), LW(10))
                CALL(btlevtcmd_BtlIconSetFallAccel, LW(10), FLOAT(0.5f))
                CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
                CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_BtlIconJumpPosition, LW(10), LW(0), LW(1), LW(2), 20)
                CALL(btlevtcmd_BtlIconDelete, LW(10))
                CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM)
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_1"))
    WAIT_FRAMES(10)
    IF_INT_EQUAL(LW(8), 0)
        WAIT_FRAMES(20)
        // If nothing was stolen, it sulks and trudges back.
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_INCLINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("B_1"))
        WAIT_FRAMES(52)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_4"))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(99)
    END_IF
    // If it got the loot it laughs and switches to its item-carrying poses.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_2"))
    WAIT_FRAMES(14)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_LAUGH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_item_have))
    GOTO(98)
    // The camera resets and the Big Bandit runs back home.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Bandit runs away with the stolen item: it turns around (SFX_ENM_BORODO_WAIT1, pose R_3), hops,
 * blurs with SFX_ENM_BORODO_ESCAPE1 and dashes 300 units off the stage at speed 9,
 * then removes itself from the battle (KillUnit), taking the item with it.
 */
static EVT_BEGIN(escape_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_WAIT1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_3"))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_JUMP1"), 0, 0, -1, -1, 0, 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(20.0f), -2)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 255, 255, 255, 255, 100, 100, 100, 50, 0)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ESCAPE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, 300, LW(1), LW(2), 0, 0, 0)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Idle event of the Bandit: uses the plain pose table,
 * or the one showing the stolen item bag if the unit flag 0x40000000 is set, and shows the matching idle pose.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_CheckUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_item_have))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Script helper for the Big Bandit's steal.
 * args[0] is the thief, args[1] the victim; args[2]-args[4] receive the item id, its icon and the amount taken.
 * 40% of the time it takes up to 10 coins, otherwise a random held item
 * or badge (equipped badges are removed from the party member wearing them).
 */
static USER_FUNC(_steal_item) {
    EvtWord* args = event->args;
    s32 coins;
    s32 num;
    s32 count;
    PouchData* pouch = pouchGetPtr();
    BattleUnit* unit;
    BattleUnit* target;
    s32 item;
    s32 r;
    s32 i;
    s32 itemVar;
    s32 iconVar;
    s32 numVar;
    s32 id;
    s32 tid;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    tid = evtGetValue(event, args[1]);
    tid = BattleTransID(event, tid);
    target = BattleGetUnitPtr(_battleWorkPointer, tid);
    if (target == NULL) {
        return EVT_RETURN_DONE;
    }
    itemVar = args[2];
    iconVar = args[3];
    numVar = args[4];
    evtSetValue(event, itemVar, 0);
    evtSetValue(event, iconVar, 0);
    evtSetValue(event, numVar, 0);
    if (irand(100) < 40) {
        coins = pouchGetCoin();
        num = 10;
        if (coins < 10) {
            num = coins;
        }
        if (num > 0) {
            evtSetValue(event, itemVar, ITEM_COIN);
#if VERSION <= VERSION_DEMO_US
            evtSetValue(event, iconVar, 394);
#else
            evtSetValue(event, iconVar, 402);
#endif
            evtSetValue(event, numVar, num);
            TST_BATTLE_UNIT(unit)->heldItem = ITEM_COIN;
            TST_BATTLE_UNIT(unit)->stolenCoins = num;
            pouchSetCoin(coins - num);
        }
        return EVT_RETURN_DONE;
    }
    {
        item = 0;
        count = 0;

        for (i = 0; i < MAX_HELD_ITEMS; i++) {
            if (pouch->heldItems[i] != 0) {
                count++;
            }
        }
        if (count <= 0) {
            return EVT_RETURN_DONE;
        }
        r = irand(count);
        if (r >= count) {
            s32 j;

            r -= count;
#if VERSION >= VERSION_JP
            sysDebugReport("unit_borodo_main.c", 673, "%d個目のバッジを盗みます\n", r + 1);
#endif
            for (j = 0; j < 200; j++) {
                if (pouch->badges[j] != 0) {
                    if (--r < 0) {
                        item = pouch->badges[j];
                        break;
                    }
                }
            }
        } else {
            s32 k;

#if VERSION >= VERSION_JP
            sysDebugReport("unit_borodo_main.c", 686, "%d個目のアイテムを盗みます\n", r + 1);
#endif
            for (k = 0; k < MAX_HELD_ITEMS; k++) {
                if (pouch->heldItems[k] != 0) {
                    if (--r < 0) {
                        item = pouch->heldItems[k];
                        break;
                    }
                }
            }
        }
        if (item != 0) {
#if VERSION >= VERSION_JP
            sysDebugReport("unit_borodo_main.c", 701, "%sを盗みました\n", msgSearch(itemDataTable[item].name));
#endif
            if (!pouchRemoveItem(item)) {
#if VERSION >= VERSION_JP
                __assert2("unit_borodo_main.c", 703, "0", "持ってるはずのないアイテムを盗んだ");
#endif
                return EVT_RETURN_DONE;
            }
            TST_BATTLE_UNIT(unit)->heldItem = item;
            if (item >= 240 && item < 339) {
                if (target->currentKind == 222) {
                    BtlUnit_EquipItem(target, 3, 0);
                } else if (target->currentKind >= 224 && target->currentKind < 231) {
                    BtlUnit_EquipItem(target, 5, 0);
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
            }
            evtSetValue(event, itemVar, item);
            evtSetValue(event, iconVar, itemDataTable[item].icon_id);
        }
    }
    return EVT_RETURN_DONE;
}
